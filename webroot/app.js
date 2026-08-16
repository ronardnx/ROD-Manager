'use strict';

const RODD = '/data/adb/modules/rod/webroot/rodd';

/* ============================================================
 * KernelSU bridge
 * ============================================================ */
let _cbId = 0;

/** Runs a shell command. Never rejects; inspect .errno. */
function exec(cmd) {
    return new Promise((resolve) => {
        const key = `_rod_cb_${Date.now()}_${_cbId++}`;
        window[key] = (errno, stdout, stderr) => {
            delete window[key];
            resolve({ errno, stdout: stdout || '', stderr: stderr || '' });
        };
        if (typeof ksu !== 'undefined' && ksu.exec) {
            try {
                ksu.exec(cmd, '{}', key);
            } catch (e) {
                delete window[key];
                resolve({ errno: 1, stdout: '', stderr: e?.message || 'exec failed' });
            }
        } else {
            resolve({ errno: 1, stdout: '', stderr: 'ksu bridge unavailable' });
        }
    });
}

const shellQuote = (s) => `'${String(s).replace(/'/g, "'\\''")}'`;

/** Invokes a rodd subcommand. Resolves with stdout, rejects on non-zero. */
async function rodd(...args) {
    const cmd = `${shellQuote(RODD)} ${args.map(shellQuote).join(' ')}`;
    const { errno, stdout, stderr } = await exec(cmd);
    if (errno !== 0) throw new Error(stderr.trim() || `exit ${errno}`);
    const out = stdout.trim();
    if (out.startsWith('ERROR:')) throw new Error(out.slice(6).trim());
    return stdout;
}

/** Invokes a rodd subcommand expected to emit JSON. */
async function roddJson(...args) {
    const raw = await rodd(...args);
    try {
        return JSON.parse(raw.trim());
    } catch {
        throw new Error('malformed response from rodd');
    }
}

/* ============================================================
 * Icons (Material Symbols outlined, 960 grid)
 * ============================================================ */
customElements.define('md-icon', class extends HTMLElement {});

const ICONS = {
    account_tree: 'M600-200v-40h-80q-33 0-56.5-23.5T440-320v-320h-80v40q0 33-23.5 56.5T280-520H160q-33 0-56.5-23.5T80-600v-160q0-33 23.5-56.5T160-840h120q33 0 56.5 23.5T360-760v40h240v-40q0-33 23.5-56.5T680-840h120q33 0 56.5 23.5T880-760v160q0 33-23.5 56.5T800-520H680q-33 0-56.5-23.5T600-600v-40h-80v320h80v-40q0-33 23.5-56.5T680-440h120q33 0 56.5 23.5T880-360v160q0 33-23.5 56.5T800-120H680q-33 0-56.5-23.5T600-200Zm80-400h120v-160H680v160Zm0 400h120v-160H680v160ZM160-600h120v-160H160v160Z',
    add: 'M440-440H240q-17 0-28.5-11.5T200-480q0-17 11.5-28.5T240-520h200v-200q0-17 11.5-28.5T480-760q17 0 28.5 11.5T520-720v200h200q17 0 28.5 11.5T760-480q0 17-11.5 28.5T720-440H520v200q0 17-11.5 28.5T480-200q-17 0-28.5-11.5T440-240v-200Z',
    apps: 'M240-160q-33 0-56.5-23.5T160-240q0-33 23.5-56.5T240-320q33 0 56.5 23.5T320-240q0 33-23.5 56.5T240-160Zm240 0q-33 0-56.5-23.5T400-240q0-33 23.5-56.5T480-320q33 0 56.5 23.5T560-240q0 33-23.5 56.5T480-160Zm240 0q-33 0-56.5-23.5T640-240q0-33 23.5-56.5T720-320q33 0 56.5 23.5T800-240q0 33-23.5 56.5T720-160ZM240-400q-33 0-56.5-23.5T160-480q0-33 23.5-56.5T240-560q33 0 56.5 23.5T320-480q0 33-23.5 56.5T240-400Zm240 0q-33 0-56.5-23.5T400-480q0-33 23.5-56.5T480-560q33 0 56.5 23.5T560-480q0 33-23.5 56.5T480-400Zm240 0q-33 0-56.5-23.5T640-480q0-33 23.5-56.5T720-560q33 0 56.5 23.5T800-480q0 33-23.5 56.5T720-400ZM240-640q-33 0-56.5-23.5T160-720q0-33 23.5-56.5T240-800q33 0 56.5 23.5T320-720q0 33-23.5 56.5T240-640Zm240 0q-33 0-56.5-23.5T400-720q0-33 23.5-56.5T480-800q33 0 56.5 23.5T560-720q0 33-23.5 56.5T480-640Zm240 0q-33 0-56.5-23.5T640-720q0-33 23.5-56.5T720-800q33 0 56.5 23.5T800-720q0 33-23.5 56.5T720-640Z',
    check_circle: 'm424-408-86-86q-11-11-28-11t-28 11q-11 11-11 28t11 28l114 114q12 12 28 12t28-12l226-226q11-11 11-28t-11-28q-11-11-28-11t-28 11L424-408Zm56 328q-83 0-156-31.5T197-197q-54-54-85.5-127T80-480q0-83 31.5-156T197-763q54-54 127-85.5T480-880q83 0 156 31.5T763-763q54 54 85.5 127T880-480q0 83-31.5 156T763-197q-54 54-127 85.5T480-80Zm0-80q134 0 227-93t93-227q0-134-93-227t-227-93q-134 0-227 93t-93 227q0 134 93 227t227 93Zm0-320Z',
    close: 'M480-424 284-228q-11 11-28 11t-28-11q-11-11-11-28t11-28l196-196-196-196q-11-11-11-28t11-28q11-11 28-11t28 11l196 196 196-196q11-11 28-11t28 11q11 11 11 28t-11 28L536-480l196 196q11 11 11 28t-11 28q-11 11-28 11t-28-11L480-424Z',
    code: 'M320-242 111-451q-6-6-8.5-13T100-480q0-8 2.5-15t8.5-13l209-209q11-11 27.5-11t28.5 12q12 12 12 28t-12 28L197-480l178 178q11 11 11 27.5T374-246q-12 12-28 12t-26-8Zm320 2q-12-12-12-28t12-28l178-178-178-178q-11-11-11-27.5t12-28.5q12-12 28-12t26 8l209 209q6 6 8.5 13t2.5 15q0 8-2.5 15t-8.5 13L706-238q-11 11-27 10.5T640-240Z',
    delete: 'M280-120q-33 0-56.5-23.5T200-200v-520q-17 0-28.5-11.5T160-760q0-17 11.5-28.5T200-800h160q0-17 11.5-28.5T400-840h160q17 0 28.5 11.5T600-800h160q17 0 28.5 11.5T800-760q0 17-11.5 28.5T760-720v520q0 33-23.5 56.5T680-120H280Zm400-600H280v520h400v-520ZM400-280q17 0 28.5-11.5T440-320v-280q0-17-11.5-28.5T400-640q-17 0-28.5 11.5T360-600v280q0 17 11.5 28.5T400-280Zm160 0q17 0 28.5-11.5T600-320v-280q0-17-11.5-28.5T560-640q-17 0-28.5 11.5T520-600v280q0 17 11.5 28.5T560-280Z',
    error: 'M480-280q17 0 28.5-11.5T520-320q0-17-11.5-28.5T480-360q-17 0-28.5 11.5T440-320q0 17 11.5 28.5T480-280Zm0-160q17 0 28.5-11.5T520-480v-160q0-17-11.5-28.5T480-680q-17 0-28.5 11.5T440-640v160q0 17 11.5 28.5T480-440Zm0 360q-83 0-156-31.5T197-197q-54-54-85.5-127T80-480q0-83 31.5-156T197-763q54-54 127-85.5T480-880q83 0 156 31.5T763-763q54 54 85.5 127T880-480q0 83-31.5 156T763-197q-54 54-127 85.5T480-80Zm0-80q134 0 227-93t93-227q0-134-93-227t-227-93q-134 0-227 93t-93 227q0 134 93 227t227 93Z',
    event: 'M200-80q-33 0-56.5-23.5T120-160v-560q0-33 23.5-56.5T200-800h40v-40q0-17 11.5-28.5T280-880q17 0 28.5 11.5T320-840v40h320v-40q0-17 11.5-28.5T680-880q17 0 28.5 11.5T720-840v40h40q33 0 56.5 23.5T840-720v560q0 33-23.5 56.5T760-80H200Zm0-80h560v-400H200v400Zm0-480h560v-80H200v80Zm280 240q-17 0-28.5-11.5T440-440q0-17 11.5-28.5T480-480q17 0 28.5 11.5T520-440q0 17-11.5 28.5T480-400Zm-160 0q-17 0-28.5-11.5T280-440q0-17 11.5-28.5T320-480q17 0 28.5 11.5T360-440q0 17-11.5 28.5T320-400Zm320 0q-17 0-28.5-11.5T600-440q0-17 11.5-28.5T640-480q17 0 28.5 11.5T680-440q0 17-11.5 28.5T640-400ZM480-240q-17 0-28.5-11.5T440-280q0-17 11.5-28.5T480-320q17 0 28.5 11.5T520-280q0 17-11.5 28.5T480-240Zm-160 0q-17 0-28.5-11.5T280-280q0-17 11.5-28.5T320-320q17 0 28.5 11.5T360-280q0 17-11.5 28.5T320-240Zm320 0q-17 0-28.5-11.5T600-280q0-17 11.5-28.5T640-320q17 0 28.5 11.5T680-280q0 17-11.5 28.5T640-240Z',
    extension: 'M352-120H200q-33 0-56.5-23.5T120-200v-152q48 0 84-30.5t36-77.5q0-47-36-77.5T120-568v-152q0-33 23.5-56.5T200-800h160q0-42 29-71t71-29q42 0 71 29t29 71h160q33 0 56.5 23.5T800-720v160q42 0 71 29t29 71q0 42-29 71t-71 29v160q0 33-23.5 56.5T720-120H568q0-50-31.5-85T460-240q-45 0-76.5 35T352-120Zm-152-80h85q24-66 77-93t98-27q45 0 98 27t77 93h85v-240h80q8 0 14-6t6-14q0-8-6-14t-14-6h-80v-240H480v-80q0-8-6-14t-14-6q-8 0-14 6t-6 14v80H200v88q54 20 87 67t33 105q0 57-33 104t-87 68v88Z',
    fingerprint: 'M134-232q-11-11-11-27.5t12-27.5q65-64 147-98.5T460-420q29 0 56 3.5t54 9.5q17 4 26 18t5 30q-4 17-18.5 25.5T551-332q-22-5-44.5-6.5T460-340q-79 0-149 29t-124 82q-11 11-26.5 11T134-232Zm546-58q-14 0-26-7t-19-20q-25-45-83-80t-125-43q-17-2-28-14t-11-29q0-17 11.5-28.5T428-522q90 8 161 51.5T698-353q10 17 1 34t-19 24q-4 3-13 3ZM261-559q-16-9-19.5-27t8.5-32q26-34 76.5-58T440-700q19 0 38 2t38 6q18 4 26 18.5t3 30.5q-5 16-19 24t-31 4q-13-3-27-4t-28-1q-59 0-97 17.5T296-556q-8 8-18 8t-17-11Zm475-101q-9 0-17-4-53-27-111-41.5T480-720q-93 0-171 33.5T176-596q-11 12-26.5 13T122-593q-12-11-12.5-27.5T120-648q66-70 158-111t202-41q77 0 145 18t124 51q15 9 18 26t-6 31q-6 9-15 14.5t-20 5.5ZM459-80q-95 0-161.5-66.5T231-308q0-90 65-151t169-61q86 0 148 55.5T675-322q0 65-45.5 110.5T520-166q-66 0-113-46.5T360-326q0-17 11.5-28.5T400-366q17 0 28.5 11.5T440-326q0 33 23.5 56.5T520-246q32 0 54.5-22.5T597-322q0-56-45-95.5T465-457q-71 0-115.5 42T305-308q0 62 45 105t109 43q13 0 25.5-1.5T510-166q17-4 31 5.5t18 26.5q4 17-5.5 30.5T527-85q-17 3-34 4t-34 1Z',
    folder: 'M160-160q-33 0-56.5-23.5T80-240v-480q0-33 23.5-56.5T160-800h207q16 0 30.5 6t25.5 17l57 57h360q33 0 56.5 23.5T920-640v400q0 33-23.5 56.5T840-160H160Zm0-80h680v-400H447l-80-80H160v480Zm0 0v-480 480Z',
    home: 'M240-200h120v-200q0-17 11.5-28.5T400-440h160q17 0 28.5 11.5T600-400v200h120v-360L480-740 240-560v360Zm-80 0v-360q0-19 8.5-36t23.5-28l240-180q21-16 48-16t48 16l240 180q15 11 23.5 28t8.5 36v360q0 33-23.5 56.5T720-120H560q-17 0-28.5-11.5T520-160v-200h-80v200q0 17-11.5 28.5T400-120H240q-33 0-56.5-23.5T160-200Z',
    info: 'M480-280q17 0 28.5-11.5T520-320v-160q0-17-11.5-28.5T480-520q-17 0-28.5 11.5T440-480v160q0 17 11.5 28.5T480-280Zm0-320q17 0 28.5-11.5T520-640q0-17-11.5-28.5T480-680q-17 0-28.5 11.5T440-640q0 17 11.5 28.5T480-600Zm0 520q-83 0-156-31.5T197-197q-54-54-85.5-127T80-480q0-83 31.5-156T197-763q54-54 127-85.5T480-880q83 0 156 31.5T763-763q54 54 85.5 127T880-480q0 83-31.5 156T763-197q-54 54-127 85.5T480-80Zm0-80q134 0 227-93t93-227q0-134-93-227t-227-93q-134 0-227 93t-93 227q0 134 93 227t227 93Z',
    key: 'M280-400q-33 0-56.5-23.5T200-480q0-33 23.5-56.5T280-560q33 0 56.5 23.5T360-480q0 33-23.5 56.5T280-400Zm0 160q-100 0-170-70T40-480q0-100 70-170t170-70q67 0 121.5 33t86.5 87h281q20 0 37.5 8.5T835-568l70 76q6 6 8.5 13.5T916-462q0 8-2.5 15t-8.5 13l-124 132q-11 11-25.5 17t-30.5 6q-14 0-27-4.5T674-297l-49-39-44 31q-11 8-24 11.5t-26 1.5q-13-2-24.5-8.5T487-320l-19-27h-20q-30 55-86 81t-82 26Zm0-80q52 0 91-30.5t53-77.5h133l50 71 87-62 86 68 79-84-40-45H424q-14-47-53-77.5T280-560q-66 0-113 47t-47 113q0 66 47 113t113 47Z',
    layers: 'M480-160 154-414q-11-8-12.5-21.5T151-460q9-11 22-12.5t24 6.5l283 220 283-220q11-8 24.5-6.5T810-460q9 11 7.5 24.5T805-414L480-160Zm0-158L154-572q-11-8-12.5-21.5T151-618q9-11 22-12.5t24 6.5l283 220 283-220q11-8 24.5-6.5T810-618q9 11 7.5 24.5T805-572L480-318Zm0-158L173-714q-16-12-16-31.5t16-31.5l307-238 307 238q16 12 16 31.5T787-714L480-476Zm0-101 178-138-178-138-178 138 178 138Z',
    memory: 'M360-400v-160q0-17 11.5-28.5T400-600h160q17 0 28.5 11.5T600-560v160q0 17-11.5 28.5T560-360H400q-17 0-28.5-11.5T360-400Zm80-40h80v-80h-80v80Zm-80 280v-40h-80q-33 0-56.5-23.5T200-280v-80h-40q-17 0-28.5-11.5T120-400q0-17 11.5-28.5T160-440h40v-80h-40q-17 0-28.5-11.5T120-560q0-17 11.5-28.5T160-600h40v-80q0-33 23.5-56.5T280-760h80v-40q0-17 11.5-28.5T400-840q17 0 28.5 11.5T440-800v40h80v-40q0-17 11.5-28.5T560-840q17 0 28.5 11.5T600-800v40h80q33 0 56.5 23.5T760-680v80h40q17 0 28.5 11.5T840-560q0 17-11.5 28.5T800-520h-40v80h40q17 0 28.5 11.5T840-400q0 17-11.5 28.5T800-360h-40v80q0 33-23.5 56.5T680-200h-80v40q0 17-11.5 28.5T560-120q-17 0-28.5-11.5T520-160v-40h-80v40q0 17-11.5 28.5T400-120q-17 0-28.5-11.5T360-160Zm320-120v-400H280v400h400Z',
    person: 'M480-480q-66 0-113-47t-47-113q0-66 47-113t113-47q66 0 113 47t47 113q0 66-47 113t-113 47ZM160-240v-32q0-34 17.5-62.5T224-378q62-31 126-46.5T480-440q66 0 130 15.5T736-378q29 15 46.5 43.5T800-272v32q0 33-23.5 56.5T720-160H240q-33 0-56.5-23.5T160-240Zm80 0h480v-32q0-11-5.5-20T700-306q-54-27-109-40.5T480-360q-56 0-111 13.5T260-306q-9 5-14.5 14t-5.5 20v32Zm240-320q33 0 56.5-23.5T560-640q0-33-23.5-56.5T480-720q-33 0-56.5 23.5T400-640q0 33 23.5 56.5T480-560Z',
    repeat: 'M280-80 136-224q-6-6-8.5-13T125-252q0-8 2.5-15t8.5-13l144-144q11-11 28-11t28 11q11 11 11 28t-11 28l-75 76h420v-120q0-17 11.5-28.5T721-452q17 0 28.5 11.5T761-412v120q0 33-23.5 56.5T681-212H261l75 76q11 11 11 28t-11 28q-11 11-28 11t-28-11ZM199-508q-17 0-28.5-11.5T159-548v-120q0-33 23.5-56.5T239-748h420l-75-76q-11-11-11-28t11-28q11-11 28-11t28 11l144 144q6 6 8.5 13t2.5 15q0 8-2.5 15t-8.5 13L640-536q-11 11-28 11t-28-11q-11-11-11-28t11-28l75-76H239v120q0 17-11.5 28.5T199-508Z',
    settings_suggest: 'M336-80q-15 0-26-10t-13-25l-8-59q-7-3-15-8t-13-10l-55 24q-14 6-28.5 1.5T155-185L91-297q-8-14-4.5-28.5T102-349l47-35v-32l-47-35q-12-9-15.5-23.5T91-503l64-112q8-14 22.5-18.5T206-632l55 24q5-5 13-10t15-8l8-59q2-15 13-25t26-10h130q15 0 26 10t13 25l8 59q7 3 15 8t13 10l55-24q14-6 28.5-1.5T647-615l64 112q8 14 4.5 28.5T700-451l-47 35v32l47 35q12 9 15.5 23.5T711-297l-64 112q-8 14-22.5 18.5T596-168l-55-24q-5 5-13 10t-15 8l-8 59q-2 15-13 25t-26 10H336Zm35-80h60l8-72q29-8 49.5-20.5T529-286l66 30 28-50-58-44q8-23 8-50t-8-50l58-44-28-50-66 30q-20-21-40.5-33.5T439-568l-8-72h-60l-8 72q-29 8-49.5 20.5T273-514l-66-30-28 50 58 44q-8 23-8.5 50t8.5 50l-58 44 28 50 66-30q20 21 40.5 33.5T363-232l8 72Zm30-240Zm0 0q0 33 23.5 56.5T481-320q33 0 56.5-23.5T561-400q0-33-23.5-56.5T481-480q-33 0-56.5 23.5T401-400Zm296-296-56-26q-12-5-12-18t12-18l56-26 26-56q5-12 18-12t18 12l26 56 56 26q12 5 12 18t-12 18l-56 26-26 56q-5 12-18 12t-18-12l-26-56Zm92 308-49-23q-6-3-6-9t6-9l49-23 23-49q3-6 9-6t9 6l23 49 49 23q6 3 6 9t-6 9l-49 23-23 49q-3 6-9 6t-9-6l-23-49Z',
    search: 'M380-320q-109 0-184.5-75.5T120-580q0-109 75.5-184.5T380-840q109 0 184.5 75.5T640-580q0 44-14 83t-38 69l224 224q11 11 11 28t-11 28q-11 11-28 11t-28-11L532-372q-30 24-69 38t-83 14Zm0-80q75 0 127.5-52.5T560-580q0-75-52.5-127.5T380-760q-75 0-127.5 52.5T200-580q0 75 52.5 127.5T380-400Z',
    shield: 'M467-85q-6-1-12-3-135-45-215-166.5T160-516v-189q0-25 14.5-45t37.5-29l240-90q14-5 28-5t28 5l240 90q23 9 37.5 29t14.5 45v189q0 140-80 261.5T505-88q-6 2-12 3t-13 1q-7 0-13-1Zm13-79q104-33 172-132t68-220v-189l-240-90-240 90v189q0 121 68 220t172 132Zm0-316Z',
    smartphone: 'M280-40q-33 0-56.5-23.5T200-120v-720q0-33 23.5-56.5T280-920h400q33 0 56.5 23.5T760-840v720q0 33-23.5 56.5T680-40H280Zm0-120v40h400v-40H280Zm0-80h400v-480H280v480Zm0-560h400v-40H280v40Zm0 0v-40 40Zm0 640v40-40Z',
    sync: 'M204-318q-22-38-33-78t-11-82q0-134 93-228t227-94h7l-73-73q-11-11-11-28t11-28q11-11 28-11t28 11l131 131q6 6 8.5 13t2.5 15q0 8-2.5 15t-8.5 13L470-710q-11 11-27.5 11T414-710q-11-11-11-28t11-28l69-69h-3q-100 0-170 71.5T240-478q0 26 6 51t18 49q8 16 4 32.5T253-319q-14 8-29.5 4T204-318Zm351 246L424-203q-6-6-8.5-13t-2.5-15q0-8 2.5-15t8.5-13l131-131q11-11 27.5-11t28.5 11q11 11 11 28t-11 28l-70 70h4q100 0 170-71.5T786-482q0-26-6-50.5T762-581q-8-16-4-32.5t18-24.5q14-8 29.5-4t23.5 19q22 38 33 78t11 82q0 134-93 228t-227 94h-6l73 73q11 11 11 28t-11 28q-11 11-28 11t-28-11Z',
    verified: 'm344-60-76-128-144-32 14-146-96-114 96-112-14-148 144-30 76-130 136 58 136-58 76 130 144 30-14 148 96 112-96 114 14 146-144 32-76 128-136-58-136 58Zm34-102 102-44 104 44 56-96 110-26-10-112 74-84-74-86 10-112-110-24-58-96-102 44-104-44-56 96-110 24 10 112-74 86 74 84-10 114 110 24 58 96Zm102-318Zm-42 142 226-226-58-58-168 168-84-84-58 58 142 142Z',
    visibility_off: 'M629-561q19 19 28.5 44t9.5 51q0 11-2 22t-6 22L494-582q10-4 21-6t21-2q26 0 51 9.5t42 19.5ZM480-720q-27 0-53 3t-51 9l-74-74q41-14 84-21t94-7q142 0 262 71.5T846-549q7 12 10 25.5t3 27.5q0 14-2.5 27T847-443q-13 29-30.5 56T778-336l-58-58q17-17 30.5-36.5T774-472q-49-90-135-149t-199-59H480Zm0 520q-142 0-262-71.5T31-451q-7-12-10-25.5T18-504q0-14 2.5-26.5T29-556q17-38 43.5-71.5T133-689L27-795q-11-11-11-27.5T28-851q11-11 28-11t28 11l736 736q11 11 11.5 27T820-60q-11 11-28 11t-28-11L636-176q-38 12-77 18t-79 6Zm-91-105q22 5 44.5 7.5T480-295q17 0 33.5-1t33.5-4L336-511q-2 15-3.5 30t-1.5 30q0 24 3 46.5t10 44.5ZM254-632q-32 25-58.5 55.5T150-508q46 87 129 145.5T458-296q-32-31-51.5-72T387-456q0-32 7-62t20-57l-160-57Z',
};

const _iconCache = {};
function setIcon(el, name) {
    if (!el || el.dataset.icon === name) return;
    const path = ICONS[name];
    if (!path) return;
    if (!_iconCache[name]) {
        _iconCache[name] = `<svg viewBox="0 -960 960 960" aria-hidden="true"><path d="${path}"/></svg>`;
    }
    el.dataset.icon = name;
    el.setAttribute('aria-hidden', 'true');
    el.innerHTML = _iconCache[name];
}

function applyIcons(root = document) {
    root.querySelectorAll('md-icon').forEach((el) => {
        const name = el.dataset.icon || el.textContent.trim();
        if (name) setIcon(el, name);
    });
}

/* ============================================================
 * Small helpers
 * ============================================================ */
const $ = (id) => document.getElementById(id);
const escapeHtml = (s) => String(s).replace(/[&<>"']/g, (c) =>
    ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]));

function setText(id, value, cls) {
    const el = $(id);
    if (!el) return;
    el.textContent = (value === undefined || value === null || value === '') ? '—' : value;
    if (cls !== undefined) el.className = 'info-value' + (cls ? ' ' + cls : '');
}

function toast(msg, isError = false) {
    const box = $('toast-container');
    if (!box) return;
    const el = document.createElement('div');
    el.className = 'toast' + (isError ? ' error' : '');
    el.textContent = msg;
    box.appendChild(el);
    setTimeout(() => {
        el.classList.add('leaving');
        setTimeout(() => el.remove(), 260);
    }, 2600);
}

const fail = (prefix, e) => toast(`${prefix}: ${e.message || e}`, true);

function relativeTime(epochSec) {
    const n = parseInt(epochSec, 10);
    if (!n || isNaN(n)) return 'Never';
    const diff = Math.floor(Date.now() / 1000) - n;
    if (diff < 5) return 'Just now';
    if (diff < 60) return `${diff} seconds ago`;
    if (diff < 120) return 'A minute ago';
    if (diff < 3600) return `${Math.floor(diff / 60)} minutes ago`;
    if (diff < 7200) return 'An hour ago';
    if (diff < 86400) return `${Math.floor(diff / 3600)} hours ago`;
    if (diff < 172800) return 'Yesterday';
    return `${Math.floor(diff / 86400)} days ago`;
}

function setTime(id, epochSec) {
    const el = $(id);
    if (!el) return;
    const n = parseInt(epochSec, 10);
    if (!n || isNaN(n)) {
        delete el.dataset.epoch;
        el.textContent = 'Never';
    } else {
        el.dataset.epoch = String(n);
        el.textContent = relativeTime(n);
    }
}

setInterval(() => {
    document.querySelectorAll('[data-epoch]').forEach((el) => {
        el.textContent = relativeTime(el.dataset.epoch);
    });
}, 30000);

/** Runs an async action while showing progress on a button. */
async function withButton(id, busyLabel, fn) {
    const btn = $(id);
    const original = btn ? btn.innerHTML : '';
    if (btn) {
        btn.disabled = true;
        btn.textContent = busyLabel;
    }
    try {
        return await fn();
    } finally {
        if (btn) {
            btn.disabled = false;
            btn.innerHTML = original;
            applyIcons(btn);
        }
    }
}

function setChip(id, state, ok) {
    const el = $(id);
    if (!el) return;
    el.querySelector('.chip-state').textContent = state;
    el.classList.toggle('ok', ok === true);
    el.classList.toggle('bad', ok === false);
}

/* ============================================================
 * Navigation
 * ============================================================ */
const VIEW_TITLES = {
    'view-home': 'ROD Manager',
    'view-susfs': 'SuSFS',
    'view-attest': 'Attestation',
    'view-hide': 'Hide',
    'view-about': 'About',
};

const pageContainer = $('page-container');
const topBar = $('top-app-bar');
const topBarTitle = $('top-app-bar-title');
let activeView = 'view-home';

function switchView(target) {
    if (target === activeView) return;
    activeView = target;
    document.querySelectorAll('.view-content').forEach((v) => v.classList.remove('active'));
    document.querySelectorAll('.nav-item').forEach((n) => {
        n.classList.toggle('active', n.dataset.target === target);
    });
    const view = $(target);
    if (view) view.classList.add('active');
    if (topBarTitle) topBarTitle.textContent = VIEW_TITLES[target] || '';
    if (pageContainer) pageContainer.scrollTop = 0;
    updateTopBar();
}

document.querySelectorAll('.nav-item').forEach((item) => {
    item.addEventListener('click', () => switchView(item.dataset.target));
});

function updateTopBar() {
    if (!pageContainer || !topBar) return;
    const y = pageContainer.scrollTop;
    const progress = Math.min(1, y / 48);
    topBar.style.setProperty('--top-app-bar-opacity', String(progress));
    topBar.style.setProperty('--top-app-title-opacity', String(Math.max(0, (y - 24) / 32)));
    topBar.classList.toggle('show-title', y > 24);
}

if (pageContainer) {
    pageContainer.addEventListener('scroll', () => {
        requestAnimationFrame(updateTopBar);
    }, { passive: true });
}

/* ============================================================
 * Modals
 * ============================================================ */
function openModal(id) {
    const m = $(id);
    if (m) m.classList.add('active');
}
function closeModal(id) {
    const m = $(id);
    if (m) m.classList.remove('active');
}

document.querySelectorAll('[data-close]').forEach((btn) => {
    btn.addEventListener('click', () => closeModal(btn.dataset.close));
});
document.querySelectorAll('.modal-overlay').forEach((overlay) => {
    overlay.addEventListener('click', (e) => {
        if (e.target === overlay) overlay.classList.remove('active');
    });
});

/* ============================================================
 * SuSFS
 * ============================================================ */
const state = {
    susfs: null,
    rules: {},
    kstat: [],
};

const RULE_FILES = ['sus_path.txt', 'sus_path_loop.txt', 'sus_maps.txt'];

function countEntries(text) {
    return String(text || '')
        .split('\n')
        .map((l) => l.trim())
        .filter((l) => l && !l.startsWith('#')).length;
}

function renderRuleCounts() {
    RULE_FILES.forEach((file) => {
        const n = countEntries(state.rules[file]);
        setText('count-' + file.replace('.txt', ''), `${n} ${n === 1 ? 'entry' : 'entries'}`);
    });
}

function renderKstat() {
    const box = $('kstat-list');
    if (!box) return;
    box.innerHTML = '';
    if (!state.kstat.length) {
        box.innerHTML = '<div class="placeholder">No KSTAT rules yet.</div>';
        return;
    }
    state.kstat.forEach((rule, idx) => {
        const spoofed = Object.keys(rule)
            .filter((k) => k !== 'path' && rule[k] && rule[k] !== 'default');
        const row = document.createElement('div');
        row.className = 'rule-row';
        row.innerHTML = `
            <div class="info-data">
              <div class="rule-title">${escapeHtml(rule.path || '(no path)')}</div>
              <div class="rule-sub">${spoofed.length ? escapeHtml(spoofed.join(', ')) : 'nothing spoofed'}</div>
            </div>
            <button class="icon-btn danger" aria-label="Delete"><md-icon>delete</md-icon></button>`;
        row.addEventListener('click', () => openKstatEditor(idx));
        const del = row.querySelector('.icon-btn');
        del.addEventListener('click', async (e) => {
            e.stopPropagation();
            state.kstat.splice(idx, 1);
            try {
                await saveKstat();
                renderKstat();
                toast('Rule deleted');
            } catch (err) {
                fail('Delete failed', err);
                await loadSusfs();
            }
        });
        box.appendChild(row);
    });
    applyIcons(box);
}

function saveKstat() {
    return rodd('susfs', 'set_rule', 'sus_kstat_statically.json',
        JSON.stringify(state.kstat, null, 2));
}

async function loadSusfs() {
    let data;
    try {
        data = await roddJson('susfs', 'status');
    } catch (e) {
        $('susfs-missing')?.classList.remove('hidden');
        setText('dev-susfs', 'Unavailable', 'bad');
        return;
    }
    state.susfs = data;
    $('susfs-missing')?.classList.toggle('hidden', !!data.supported);

    document.querySelectorAll('.md-switch[data-key]').forEach((sw) => {
        const key = sw.dataset.key;
        if (key in data) sw.querySelector('.switch-input').checked = !!data[key];
    });

    const rel = data.kernel_version && data.kernel_version !== 'default' ? data.kernel_version : '';
    const build = data.kernel_build && data.kernel_build !== 'default' ? data.kernel_build : '';
    setText('uname-summary', rel || build ? [rel, build].filter(Boolean).join(' · ') : 'Automatic');
    if ($('uname-release')) $('uname-release').value = rel;
    if ($('uname-build')) $('uname-build').value = build;

    const [p, l, m, k] = await Promise.all([
        rodd('susfs', 'get_rule', 'sus_path.txt').catch(() => ''),
        rodd('susfs', 'get_rule', 'sus_path_loop.txt').catch(() => ''),
        rodd('susfs', 'get_rule', 'sus_maps.txt').catch(() => ''),
        rodd('susfs', 'get_rule', 'sus_kstat_statically.json').catch(() => ''),
    ]);
    state.rules = { 'sus_path.txt': p, 'sus_path_loop.txt': l, 'sus_maps.txt': m };
    renderRuleCounts();

    try {
        const parsed = k.trim() ? JSON.parse(k) : [];
        state.kstat = Array.isArray(parsed) ? parsed : [parsed];
    } catch {
        state.kstat = [];
    }
    renderKstat();
}

/* --- toggles --- */
document.querySelectorAll('.md-switch[data-key]').forEach((sw) => {
    const input = sw.querySelector('.switch-input');
    input.addEventListener('change', async () => {
        const key = sw.dataset.key;
        const value = input.checked;
        sw.classList.add('busy');
        try {
            await rodd('susfs', 'toggle', key, value ? 'true' : 'false');
            toast(value ? 'Enabled' : 'Disabled');
        } catch (e) {
            input.checked = !value;
            fail('Failed', e);
        } finally {
            sw.classList.remove('busy');
        }
    });
});

/* --- rule editor --- */
let currentRuleFile = '';

document.querySelectorAll('[data-rule]').forEach((row) => {
    row.addEventListener('click', () => {
        currentRuleFile = row.dataset.rule;
        $('modal-rule-title').textContent = row.dataset.ruleTitle || 'Edit rule';
        $('rule-text').value = state.rules[currentRuleFile] || '';
        openModal('modal-rule');
    });
});

$('btn-rule-save')?.addEventListener('click', async () => {
    await withButton('btn-rule-save', 'Saving…', async () => {
        try {
            const value = $('rule-text').value;
            await rodd('susfs', 'set_rule', currentRuleFile, value);
            state.rules[currentRuleFile] = value;
            renderRuleCounts();
            closeModal('modal-rule');
            toast('Rules saved · reboot to fully apply');
        } catch (e) {
            fail('Save failed', e);
        }
    });
});

/* --- uname editor --- */
$('row-uname')?.addEventListener('click', () => openModal('modal-uname'));

$('btn-uname-save')?.addEventListener('click', async () => {
    await withButton('btn-uname-save', 'Saving…', async () => {
        try {
            const rel = $('uname-release').value.trim() || 'default';
            const build = $('uname-build').value.trim() || 'default';
            await rodd('susfs', 'set_val', 'kernel_version', rel);
            await rodd('susfs', 'set_val', 'kernel_build', build);
            closeModal('modal-uname');
            toast('Kernel identity saved');
            await loadSusfs();
        } catch (e) {
            fail('Save failed', e);
        }
    });
});

/* --- KSTAT editor --- */
const KSTAT_FIELDS = ['ino', 'dev', 'nlink', 'size', 'blocks', 'blksize',
    'atime', 'atime_nsec', 'mtime', 'mtime_nsec', 'ctime', 'ctime_nsec'];
let currentKstatIdx = -1;

function openKstatEditor(idx) {
    currentKstatIdx = idx;
    const rule = idx >= 0 ? state.kstat[idx] : null;
    $('modal-kstat-title').textContent = idx >= 0 ? 'Edit KSTAT rule' : 'New KSTAT rule';
    $('btn-kstat-delete').style.display = idx >= 0 ? '' : 'none';
    const clean = (v) => (!v || v === 'default') ? '' : v;
    $('kstat-path').value = rule ? clean(rule.path) : '';
    KSTAT_FIELDS.forEach((f) => {
        const el = $('kstat-' + f);
        if (el) el.value = rule ? clean(rule[f]) : '';
    });
    openModal('modal-kstat');
}

$('btn-kstat-add')?.addEventListener('click', () => openKstatEditor(-1));

$('btn-kstat-save')?.addEventListener('click', async () => {
    const path = $('kstat-path').value.trim();
    if (!path) { toast('Path cannot be empty', true); return; }
    const rule = { path };
    KSTAT_FIELDS.forEach((f) => {
        rule[f] = $('kstat-' + f)?.value.trim() || 'default';
    });
    await withButton('btn-kstat-save', 'Saving…', async () => {
        const backup = state.kstat.slice();
        if (currentKstatIdx >= 0) state.kstat[currentKstatIdx] = rule;
        else state.kstat.push(rule);
        try {
            await saveKstat();
            closeModal('modal-kstat');
            renderKstat();
            toast('KSTAT rule saved');
        } catch (e) {
            state.kstat = backup;
            fail('Save failed', e);
        }
    });
});

$('btn-kstat-delete')?.addEventListener('click', async () => {
    if (currentKstatIdx < 0) return;
    const backup = state.kstat.slice();
    state.kstat.splice(currentKstatIdx, 1);
    try {
        await saveKstat();
        closeModal('modal-kstat');
        renderKstat();
        toast('Rule deleted');
    } catch (e) {
        state.kstat = backup;
        fail('Delete failed', e);
    }
});

/* Fills the KSTAT form with the real stat() values of another path, which is
 * the usual way to make a hidden path look like an innocent one. */
$('btn-kstat-clone')?.addEventListener('click', async () => {
    const src = prompt('Copy stat values from which path?', '/system/bin');
    if (!src) return;
    await withButton('btn-kstat-clone', 'Cloning…', async () => {
        const result = await roddJson('kstat', 'stat', src);
        Object.entries(result).forEach(([key, val]) => {
            const input = $(`kstat-${key}`);
            if (input) input.value = val;
        });
    });
});
