'use strict';
const MODDIR      = '/data/adb/modules/rod';
const SCRIPTS_DIR = MODDIR + '/webroot';
function exec(command) {
  return new Promise((resolve, reject) => {
    const id = `exec_${Date.now()}_${Math.random().toString(36).slice(2)}`;
    window[id] = (errno, stdout, stderr) => {
      delete window[id];
      if (errno !== 0) reject(new Error(stderr || `Exit code ${errno}`));
      else resolve(stdout || '');
    };
    try { ksu.exec(command, '{}', id); } 
    catch (e) { delete window[id]; reject(new Error('ksu not available: ' + e.message)); }
  });
}
function sh(script, ...args) {
  const submodule = script.replace(/\.sh$/, '');
  const escaped = args.map(a => `'${String(a).replace(/'/g, "'\\''")}'`).join(' ');
  return exec(`'${SCRIPTS_DIR}/rodd' ${submodule} ${escaped}`);
}
function toast(msg, type = '') {
  const lmsg = msg.toLowerCase();
  if (lmsg.includes('failed') || lmsg.includes('error')) type = 'error';
  else if (lmsg.includes('success') || lmsg.includes('saved') || lmsg.includes('applied') || lmsg.includes('configured') || lmsg.includes('targeted') || lmsg.includes('updated') || lmsg.includes('deleted')) type = 'success';
  
  const container = document.getElementById('toast-container');
  const el = document.createElement('div');
  el.className = 'toast ' + type;
  el.textContent = msg;
  container.appendChild(el);
  setTimeout(() => el.remove(), 3000);
}
function setText(id, value) {
  const el = document.getElementById(id);
  if (el) el.textContent = value || '—';
}
function setTime(id, epochSec) {
  const el = document.getElementById(id);
  if (el) {
    if (!epochSec || isNaN(epochSec)) {
      el.textContent = 'Never';
      delete el.dataset.epoch;
    } else {
      el.dataset.epoch = epochSec;
      el.textContent = relativeTime(epochSec);
    }
  }
}
setInterval(() => {
  document.querySelectorAll('[data-epoch]').forEach(el => {
    el.textContent = relativeTime(el.dataset.epoch);
  });
}, 1000);
function setBtnLoading(btnId, isLoading, originalText) {
  const btn = document.getElementById(btnId);
  if (!btn) return;
  btn.disabled = isLoading;
  btn.textContent = isLoading ? 'Processing...' : originalText;
  if (isLoading) btn.classList.add('loading');
  else btn.classList.remove('loading');
}
function relativeTime(epochSec) {
  if (!epochSec || isNaN(epochSec)) return 'Never';
  const diff = Math.floor(Date.now() / 1000) - parseInt(epochSec, 10);
  if (diff < 60) return `${diff} seconds ago`;
  if (diff < 3600) return `${Math.floor(diff / 60)} minutes ago`;
  if (diff < 86400) return `${Math.floor(diff / 3600)} hours ago`;
  return `${Math.floor(diff / 86400)} days ago`;
}
const navBtns = document.querySelectorAll('.nav_button');
const panels = document.querySelectorAll('.panel');
navBtns.forEach(btn => {
  btn.addEventListener('click', () => {
    const target = btn.dataset.target;
    navBtns.forEach(b => b.classList.remove('active'));
    panels.forEach(p => p.classList.remove('active'));
    btn.classList.add('active');
    document.getElementById(target).classList.add('active');
  });
});
async function loadTeeSimulator() {
  try {
    const raw = await sh('teesimulator.sh', 'status');
    const data = JSON.parse(raw.trim());
    if (!data.installed) {
      document.getElementById('teesim-missing')?.classList.remove('hidden');
      document.getElementById('teesim-main-card')?.classList.add('hidden');
      const hTs = document.getElementById('home-ts-status');
      if (hTs) { hTs.textContent = 'Not Installed'; hTs.style.color = 'var(--danger)'; }
      return;
    } else {
      document.getElementById('teesim-missing')?.classList.add('hidden');
      document.getElementById('teesim-main-card')?.classList.remove('hidden');
      const hTs = document.getElementById('home-ts-status');
      if (hTs) { hTs.textContent = 'Active'; hTs.style.color = 'var(--success)'; }
    }
    const keyboxEl = document.getElementById('teesim-keybox-status');
    const verifyBtn = document.getElementById('teesim-verify-btn');
    if (keyboxEl) {
      keyboxEl.textContent = data.has_keybox ? '✅ Present' : '❌ Missing — import one below!';
      keyboxEl.style.color = data.has_keybox ? '' : 'var(--color-warn, #f90)';
      if (verifyBtn) verifyBtn.style.display = data.has_keybox ? 'block' : 'none';
    }
    
    const hashEl = document.getElementById('teesim-boot-hash');
    if (hashEl) {
      let hashStr = data.boot_hash || 'None (Unlocked/Not verified)';
      if (data.applied_hash) {
        hashStr += `\nApplied: ${data.applied_hash}`;
      }
      hashEl.textContent = hashStr;
    }
  } catch (e) {
    document.getElementById('teesim-missing')?.classList.remove('hidden');
    document.getElementById('teesim-main-card')?.classList.add('hidden');
  }
}

document.getElementById('teesim-verify-btn')?.addEventListener('click', async () => {
  const btn = document.getElementById('teesim-verify-btn');
  const og = btn.textContent;
  btn.textContent = 'Checking...';
  btn.disabled = true;
  try {
    const raw = await sh('teesimulator.sh', 'check_revocation');
    const res = raw.trim();
    if (res === 'REVOKED') {
      document.getElementById('teesim-keybox-status').textContent = '✅ Present (REVOKED)';
      document.getElementById('teesim-keybox-status').style.color = 'var(--color-warn, #f90)';
      toast('Warning: This keybox is blacklisted by Google!');
    } else if (res === 'VALID') {
      document.getElementById('teesim-keybox-status').textContent = '✅ Present (VALID)';
      document.getElementById('teesim-keybox-status').style.color = 'var(--color-success, #0f0)';
      toast('Success: Keybox is valid!');
    } else {
      throw new Error(res);
    }
  } catch(e) {
    toast('Error checking status: ' + e.message);
  } finally {
    btn.textContent = og;
    btn.disabled = false;
  }
});

document.getElementById('teesim-set-boothash-btn')?.addEventListener('click', async () => {
  setBtnLoading('teesim-set-boothash-btn', true, 'Applying...');
  try {
    const res = await sh('teesimulator.sh', 'apply_boot_hash');
    toast('Boot hash applied: ' + res.trim().substring(0, 8) + '...');
    await loadTeeSimulator();
  } catch(e) {
    toast('Failed to apply: ' + e.message);
  } finally {
    setBtnLoading('teesim-set-boothash-btn', false, 'Apply Boot Hash');
  }
});

document.getElementById('teesim-fetch-btn')?.addEventListener('click', async () => {
  setBtnLoading('teesim-fetch-btn', true, 'Fetching Catalog...');
  try {
    const rawJson = await sh('teesimulator.sh', 'fetch_catalog');
    const specterCatalog = JSON.parse(rawJson.trim());
    
    const container = document.getElementById('kb-list-container');
    if (container) container.innerHTML = '';
    
    const items = specterCatalog.workingEntries || specterCatalog.entries || [];
    if (items.length === 0) throw new Error("Catalog is empty");
    
    items.forEach(kb => {
      const div = document.createElement('div');
      div.className = 'kb-item';
      div.innerHTML = `
        <div class="kb-info">
          <div class="kb-source">${kb.source}</div>
          <div class="kb-version">Version ${kb.version} ${kb.text ? '('+kb.text+')' : ''}</div>
        </div>
        <button class="btn-sm btn-primary">GET</button>
      `;
      
      div.onclick = async () => {
        document.getElementById('kb-selector-modal').classList.add('hidden');
        setBtnLoading('teesim-fetch-btn', true, 'Downloading...');
        try {
          await sh('teesimulator.sh', 'fetch_specter_keybox', kb.source, kb.version);
          toast(`Keybox ${kb.source} v${kb.version} installed!`);
          await loadTeeSimulator();
        } catch(e) {
          toast('Failed to download: ' + e.message);
        } finally {
          setBtnLoading('teesim-fetch-btn', false, 'Fetch Specter Keybox');
        }
      };
      if (container) container.appendChild(div);
    });
    
    document.getElementById('kb-selector-modal').classList.remove('hidden');
  } catch(e) {
    toast('Failed to fetch catalog: ' + e.message);
  } finally {
    setBtnLoading('teesim-fetch-btn', false, 'Fetch Specter Keybox');
  }
});

document.getElementById('teesim-keybox-btn')?.addEventListener('click', () => {
  document.getElementById('teesim-keybox-upload').click();
});
document.getElementById('teesim-keybox-upload')?.addEventListener('change', async (e) => {
  const file = e.target.files[0];
  if (!file) return;
  setBtnLoading('teesim-keybox-btn', true, 'Importing...');
  try {
    const reader = new FileReader();
    reader.onload = async (evt) => {
      try {
        const base64Data = evt.target.result.split(',')[1];
        if (!base64Data) throw new Error('Invalid file content');
        await sh('teesimulator.sh', 'import_keybox', base64Data);
        toast('keybox.xml imported successfully!');
        await loadTeeSimulator();
      } catch (err) {
        toast('Import failed: ' + err.message);
      } finally {
        setBtnLoading('teesim-keybox-btn', false, 'Import keybox.xml');
        e.target.value = '';
      }
    };
    reader.onerror = () => {
      toast('Failed to read file locally');
      setBtnLoading('teesim-keybox-btn', false, 'Import keybox.xml');
      e.target.value = '';
    };
    reader.readAsDataURL(file);
  } catch (err) {
    toast('Failed: ' + err.message);
    setBtnLoading('teesim-keybox-btn', false, 'Import keybox.xml');
    e.target.value = '';
  }
});
async function loadPif() {
  try {
    const raw = await sh('pif.sh', 'status');
    const data = JSON.parse(raw.trim());
    if (!data.installed) {
      document.getElementById('pif-missing')?.classList.remove('hidden');
      document.getElementById('pif-main-card')?.classList.add('hidden');
      const hPif = document.getElementById('home-pif-status');
      if (hPif) { hPif.textContent = 'Not Installed'; hPif.style.color = 'var(--danger)'; }
      return;
    }
    const hPif = document.getElementById('home-pif-status');
    if (hPif) { hPif.textContent = 'v' + data.version; hPif.style.color = 'var(--success)'; }
    setText('pif-version', data.version);
    setTime('pif-last-fetch', data.last_fetch);
  } catch (e) {
    document.getElementById('pif-missing')?.classList.remove('hidden');
  }
}
document.getElementById('pif-fetch-btn')?.addEventListener('click', async () => {
  setBtnLoading('pif-fetch-btn', true, 'Fetch Latest pif.prop');
  try {
    await sh('pif.sh', 'fetch');
    toast('pif.prop updated successfully!');
    await loadPif();
  } catch (e) {
    toast('Fetch failed: ' + e.message);
  } finally {
    setBtnLoading('pif-fetch-btn', false, 'Fetch Latest pif.prop');
  }
});
async function loadHma() {
  try {
    const raw = await sh('hma.sh', 'status');
    const data = JSON.parse(raw.trim());
    if (!data.module_installed) {
      document.getElementById('hma-missing')?.classList.remove('hidden');
      document.getElementById('hma-main-card')?.classList.add('hidden');
      const hHma = document.getElementById('home-hma-status');
      if (hHma) { hHma.textContent = 'Not Installed'; hHma.style.color = 'var(--danger)'; }
      return;
    }
    const hHma = document.getElementById('home-hma-status');
    if (hHma) { hHma.textContent = 'Active'; hHma.style.color = 'var(--success)'; }
    setText('hma-status', 'Active & Ready');
    setTime('hma-last-apply', data.last_apply);
  } catch (err) {
    document.getElementById('hma-missing')?.classList.remove('hidden');
  }
}
document.getElementById('hma-apply-btn')?.addEventListener('click', async () => {
  setBtnLoading('hma-apply-btn', true, 'Apply Default Presets');
  try {
    await sh('hma.sh', 'apply_presets');
    toast('Fresh presets applied to all user apps!');
    await loadHma();
  } catch (e) {
    toast('Apply failed: ' + e.message);
  } finally {
    setBtnLoading('hma-apply-btn', false, 'Apply Default Presets');
  }
});
async function loadSusfs() {
  try {
    const raw = await sh('susfs.sh', 'status');
    const data = JSON.parse(raw.trim());
    if (!data.supported) {
      document.getElementById('susfs-missing')?.classList.remove('hidden');
      document.getElementById('susfs-main-card')?.classList.add('hidden');
      return;
    }
    if(document.getElementById('susfs-mnts')) document.getElementById('susfs-mnts').checked = data.hide_sus_mnts_for_non_su_procs;
    if(document.getElementById('susfs-cmdline')) document.getElementById('susfs-cmdline').checked = data.spoof_cmdline;
    if(document.getElementById('susfs-uname')) document.getElementById('susfs-uname').checked = data.spoof_uname;
    if(document.getElementById('susfs-avc')) document.getElementById('susfs-avc').checked = data.enable_avc_log_spoofing;
    if(document.getElementById('susfs-sucompat')) document.getElementById('susfs-sucompat').checked = data.su_compat;
    if(document.getElementById('susfs-umount')) document.getElementById('susfs-umount').checked = data.kernel_umount;
    if(document.getElementById('susfs-selinuxhide')) document.getElementById('susfs-selinuxhide').checked = data.selinux_hide;
    if(document.getElementById('susfs-devopt')) document.getElementById('susfs-devopt').checked = data.developer_options;
    if(document.getElementById('susfs-usbdebug')) document.getElementById('susfs-usbdebug').checked = data.usb_debugging;
    if(document.getElementById('susfs-wifidebug')) document.getElementById('susfs-wifidebug').checked = data.wireless_debugging;
    if(document.getElementById('susfs-enforce')) document.getElementById('susfs-enforce').checked = data.selinux_enforcing;
    window.susfsRules = {
      'sus_path.txt': await sh('susfs.sh', 'get_rule', 'sus_path.txt'),
      'sus_path_loop.txt': await sh('susfs.sh', 'get_rule', 'sus_path_loop.txt'),
      'sus_maps.txt': await sh('susfs.sh', 'get_rule', 'sus_maps.txt')
    };
    try {
      window.kstatRules = [];
      const kstatRaw = await sh('susfs.sh', 'get_rule', 'sus_kstat_statically.json');
      if (kstatRaw.trim()) {
        const parsed = JSON.parse(kstatRaw);
        if (Array.isArray(parsed)) {
          window.kstatRules = parsed;
        } else {
          window.kstatRules = [parsed]; 
        }
      }
    } catch(e) { console.error('KSTAT parse error', e); window.kstatRules = []; }
    refreshKstatDropdown();
  } catch (e) {
    console.error(e);
    document.getElementById('susfs-missing')?.classList.remove('hidden');
    document.getElementById('susfs-main-card')?.classList.add('hidden');
  }
}
function bindSusfsToggle(id, key) {
  const el = document.getElementById(id);
  if (!el) return;
  el.addEventListener('change', async (e) => {
    try {
      await sh('susfs.sh', 'toggle', key, e.target.checked);
      toast('SuSFS rule updated & applied!');
      await loadSusfs();
    } catch (err) {
      toast('Failed: ' + err.message);
      e.target.checked = !e.target.checked;
    }
  });
}
function bindSusfsInput(id, key) {
  const el = document.getElementById(id);
  if (!el) return;
  el.addEventListener('change', async (e) => {
    try {
      const val = e.target.value.trim() || 'default';
      await sh('susfs.sh', 'set_val', key, val);
      toast('Kernel config updated!');
      await loadSusfs();
    } catch (err) {
      toast('Failed: ' + err.message);
    }
  });
}
function bindSusfsRule(btnId, txtId, filename) {
  const btn = document.getElementById(btnId);
  const txt = document.getElementById(txtId);
  if (!btn || !txt) return;
  btn.addEventListener('click', async () => {
    try {
      const originalText = btn.textContent;
      btn.textContent = 'Saving...';
      btn.disabled = true;
      await sh('susfs.sh', 'set_rule', filename, txt.value);
      toast(filename + ' saved!');
      btn.textContent = originalText;
      btn.disabled = false;
      await loadSusfs();
    } catch (err) {
      toast('Failed: ' + err.message);
      btn.textContent = 'Save';
      btn.disabled = false;
    }
  });
}
bindSusfsToggle('susfs-mnts',    'hide_sus_mnts_for_non_su_procs');
bindSusfsToggle('susfs-cmdline', 'spoof_cmdline');
bindSusfsToggle('susfs-uname',   'spoof_uname');
const btnEditPath = document.getElementById('btn-edit-path');
const btnEditLoop = document.getElementById('btn-edit-loop');
const btnEditMaps = document.getElementById('btn-edit-maps');
const ruleModal = document.getElementById('rule-edit-modal');
const ruleTitle = document.getElementById('rule-edit-title');
const ruleTextarea = document.getElementById('rule-edit-textarea');
const btnSaveRuleModal = document.getElementById('btn-save-rule-modal');
window.currentRuleFile = '';

function openRuleModal(title, filename) {
  window.currentRuleFile = filename;
  ruleTitle.textContent = title;
  ruleTextarea.value = (window.susfsRules && window.susfsRules[filename]) ? window.susfsRules[filename] : '';
  ruleModal.classList.remove('hidden');
  
  // Auto-grow logic trigger immediately
  ruleTextarea.style.height = 'auto';
  ruleTextarea.style.height = (ruleTextarea.scrollHeight) + 'px';
  ruleTextarea.focus();
}

if(btnEditPath) btnEditPath.addEventListener('click', () => openRuleModal('Edit Paths', 'sus_path.txt'));
if(btnEditLoop) btnEditLoop.addEventListener('click', () => openRuleModal('Edit Loops', 'sus_path_loop.txt'));
if(btnEditMaps) btnEditMaps.addEventListener('click', () => openRuleModal('Edit Maps', 'sus_maps.txt'));

if(ruleTextarea) {
  ruleTextarea.addEventListener('input', function() {
    this.style.height = 'auto';
    this.style.height = (this.scrollHeight) + 'px';
  });
}

if(btnSaveRuleModal && ruleTextarea) {
  btnSaveRuleModal.addEventListener('click', async () => {
    try {
      const originalText = btnSaveRuleModal.textContent;
      btnSaveRuleModal.textContent = 'Saving...';
      btnSaveRuleModal.disabled = true;
      await sh('susfs.sh', 'set_rule', window.currentRuleFile, ruleTextarea.value);
      toast(window.currentRuleFile + ' saved!');
      btnSaveRuleModal.textContent = originalText;
      btnSaveRuleModal.disabled = false;
      ruleModal.classList.add('hidden');
      await loadSusfs();
    } catch (err) {
      toast('Failed: ' + err.message);
      btnSaveRuleModal.textContent = 'Save Rule';
      btnSaveRuleModal.disabled = false;
    }
  });
}
let currentKstatIdx = '';
const btnKstatSelect = document.getElementById('btn-kstat-select');
const kstatSelectLabel = document.getElementById('kstat-select-label');
const kstatModal = document.getElementById('kstat-rule-modal');
const kstatRuleList = document.getElementById('kstat-rule-list');
const btnKstat = document.getElementById('btn-save-kstat');
const btnKstatNew = document.getElementById('btn-kstat-new');
const btnKstatDel = document.getElementById('btn-kstat-delete');

if (btnKstatSelect) {
  btnKstatSelect.addEventListener('click', () => {
    if (kstatModal) kstatModal.classList.remove('hidden');
  });
}

function refreshKstatDropdown() {
  if (!kstatRuleList) return;
  kstatRuleList.innerHTML = '';
  
  // Default "Select Rule" item
  const defaultDiv = document.createElement('div');
  defaultDiv.className = 'kb-item';
  defaultDiv.innerHTML = `
    <div class="kb-info">
      <div class="kb-source">-- Select Rule --</div>
      <div class="kb-version">Clear selection / Create new</div>
    </div>
  `;
  defaultDiv.onclick = () => {
    currentKstatIdx = '';
    if (kstatSelectLabel) kstatSelectLabel.textContent = '-- Select Rule --';
    clearKstatForm();
    if (kstatModal) kstatModal.classList.add('hidden');
  };
  kstatRuleList.appendChild(defaultDiv);

  // List existing rules
  (window.kstatRules || []).forEach((rule, idx) => {
    const div = document.createElement('div');
    div.className = 'kb-item';
    div.innerHTML = `
      <div class="kb-info" style="overflow:hidden; text-overflow:ellipsis;">
        <div class="kb-source" style="word-break:break-all;">${rule.path}</div>
        <div class="kb-version">Rule #${idx + 1}</div>
      </div>
      <button class="btn-sm btn-primary">SELECT</button>
    `;
    div.onclick = () => {
      currentKstatIdx = idx;
      if (kstatSelectLabel) kstatSelectLabel.textContent = rule.path;
      loadKstatForm(idx);
      if (kstatModal) kstatModal.classList.add('hidden');
    };
    kstatRuleList.appendChild(div);
  });
  
  clearKstatForm();
}

function clearKstatForm() {
  if(!document.getElementById('kstat-path')) return;
  document.getElementById('kstat-path').value = '';
  document.getElementById('kstat-ino').value = '';
  document.getElementById('kstat-dev').value = '';
  document.getElementById('kstat-nlink').value = '';
  document.getElementById('kstat-size').value = '';
  document.getElementById('kstat-blocks').value = '';
  document.getElementById('kstat-blksize').value = '';
  document.getElementById('kstat-atime').value = '';
  document.getElementById('kstat-atime_nsec').value = '';
  document.getElementById('kstat-mtime').value = '';
  document.getElementById('kstat-mtime_nsec').value = '';
  document.getElementById('kstat-ctime').value = '';
  document.getElementById('kstat-ctime_nsec').value = '';
  currentKstatIdx = '';
  if (kstatSelectLabel) kstatSelectLabel.textContent = '-- Select Rule --';
}

function loadKstatForm(idx) {
  const rule = window.kstatRules[idx];
  if(!rule) return;
  const parseDef = (v) => (v === 'default' || v === undefined) ? '' : v;
  document.getElementById('kstat-path').value = parseDef(rule.path);
  document.getElementById('kstat-ino').value = parseDef(rule.ino);
  document.getElementById('kstat-dev').value = parseDef(rule.dev);
  document.getElementById('kstat-nlink').value = parseDef(rule.nlink);
  document.getElementById('kstat-size').value = parseDef(rule.size);
  document.getElementById('kstat-blocks').value = parseDef(rule.blocks);
  document.getElementById('kstat-blksize').value = parseDef(rule.blksize);
  document.getElementById('kstat-atime').value = parseDef(rule.atime);
  document.getElementById('kstat-atime_nsec').value = parseDef(rule.atime_nsec);
  document.getElementById('kstat-mtime').value = parseDef(rule.mtime);
  document.getElementById('kstat-mtime_nsec').value = parseDef(rule.mtime_nsec);
  document.getElementById('kstat-ctime').value = parseDef(rule.ctime);
  document.getElementById('kstat-ctime_nsec').value = parseDef(rule.ctime_nsec);
}

if(btnKstatNew) {
  btnKstatNew.addEventListener('click', () => {
    clearKstatForm();
    document.getElementById('kstat-path').focus();
  });
}
if(btnKstatDel) {
  btnKstatDel.addEventListener('click', async () => {
    if(currentKstatIdx === '' || currentKstatIdx === undefined) {
      toast('Please select a rule to delete');
      return;
    }
    window.kstatRules.splice(currentKstatIdx, 1);
    await sh('susfs.sh', 'set_rule', 'sus_kstat_statically.json', JSON.stringify(window.kstatRules, null, 2));
    refreshKstatDropdown();
    toast('Rule deleted!');
    await loadSusfs();
  });
}
if(btnKstat) {
  btnKstat.addEventListener('click', async () => {
    try {
      const getDef = (id) => document.getElementById(id).value.trim() || 'default';
      const pathVal = document.getElementById('kstat-path').value.trim();
      if(!pathVal) throw new Error('Path cannot be empty');
      const kstat = {
        path: pathVal,
        ino: getDef('kstat-ino'),
        dev: getDef('kstat-dev'),
        nlink: getDef('kstat-nlink'),
        size: getDef('kstat-size'),
        atime: getDef('kstat-atime'),
        atime_nsec: getDef('kstat-atime_nsec'),
        mtime: getDef('kstat-mtime'),
        mtime_nsec: getDef('kstat-mtime_nsec'),
        ctime: getDef('kstat-ctime'),
        ctime_nsec: getDef('kstat-ctime_nsec'),
        blocks: getDef('kstat-blocks'),
        blksize: getDef('kstat-blksize')
      };
      if(currentKstatIdx === '' || currentKstatIdx === undefined) {
        window.kstatRules.push(kstat);
        currentKstatIdx = window.kstatRules.length - 1;
      } else {
        window.kstatRules[currentKstatIdx] = kstat;
      }
      const originalText = btnKstat.textContent;
      btnKstat.textContent = 'Saving...';
      btnKstat.disabled = true;
      await sh('susfs.sh', 'set_rule', 'sus_kstat_statically.json', JSON.stringify(window.kstatRules, null, 2));
      refreshKstatDropdown();
      toast('KSTAT Rule Saved!');
      btnKstat.textContent = originalText;
      btnKstat.disabled = false;
      await loadSusfs();
    } catch(err) {
      toast('Failed: ' + err.message);
      if (btnKstat) {
        btnKstat.textContent = 'Save Target Properties';
        btnKstat.disabled = false;
      }
    }
  });
}
async function loadZn() {
  let raw = '';
  try {
    raw = await sh('zn.sh', 'status');
    const data = JSON.parse(raw.trim());
    if (!data.installed) {
      document.getElementById('zn-missing')?.classList.remove('hidden');
      document.getElementById('zn-main-card')?.classList.add('hidden');
      const hZn = document.getElementById('home-zn-status');
      if (hZn) { hZn.textContent = 'Not Installed'; hZn.style.color = 'var(--danger)'; }
      return;
    }
    const hZn = document.getElementById('home-zn-status');
    if (hZn) { hZn.textContent = 'v' + data.version; hZn.style.color = 'var(--success)'; }
    if(data.last_apply && data.last_apply !== "") {
      setTime('zn-last-apply', data.last_apply);
    } else {
      setTime('zn-last-apply', null);
    }
    setText('zn-version', data.version);
    setText('zn-state', data.state);
  } catch (err) {
    console.error('loadZn error:', err);
    document.getElementById('zn-missing')?.classList.remove('hidden');
    document.getElementById('zn-main-card')?.classList.add('hidden');
    const desc = document.querySelector('#zn-missing .desc');
    if (desc) desc.textContent = 'Error: ' + err.message + '\nRaw Output: ' + (raw || 'none');
  }
}
document.getElementById('zn-apply-btn')?.addEventListener('click', async () => {
  setBtnLoading('zn-apply-btn', true, 'Apply Recommended Config');
  try {
    await sh('zn.sh', 'apply');
    toast('Zygisk Next configured!');
    await loadZn();
  } catch (e) {
    toast('Failed: ' + e.message);
  } finally {
    setBtnLoading('zn-apply-btn', false, 'Apply Recommended Config');
  }
});
async function loadHome() {
  try {
    const modProp = await exec('cat /data/adb/modules/rod/module.prop 2>/dev/null || echo ""');
    let modVer = 'Unknown';
    if(modProp) {
      const match = modProp.match(/version=(.*)/);
      if(match) modVer = match[1].trim();
    }
    setText('home-mod-version', 'Version ' + modVer);
  } catch(e) {}
  
  try {
    const androidVer = await exec('getprop ro.build.version.release');
    setText('home-android-ver', androidVer.trim() || '—');
  } catch(e) {}
  
  try {
    const secPatch = await exec('getprop ro.build.version.security_patch');
    setText('home-sec-patch', secPatch.trim() || '—');
  } catch(e) {}
  
  try {
    const kernelVer = await exec('uname -r');
    setText('home-kernel-ver', kernelVer.trim() || '—');
  } catch(e) {}
  
  try {
    let susfsVer = await exec('cat /sys/fs/susfs/version 2>/dev/null || echo ""');
    if(!susfsVer.trim()) {
      susfsVer = await exec('dmesg | grep "susfs v" | head -n 1 | awk \'{print $3}\' 2>/dev/null || echo ""');
    }
    setText('home-susfs-ver', susfsVer.trim() || 'Not Found');
  } catch(e) {}
}

async function init() {
  await loadHome();
  await Promise.allSettled([loadTeeSimulator(), loadPif(), loadHma(), loadSusfs(), loadZn()]);
}
if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', init);
} else {
  init();
}

// === App Selector Modal Logic ===
let installedApps = [];
let selectedAppsSet = new Set();
let appObserver = null;
let appSelectorMode = 'hma'; // 'hma' or 'teesim'

async function openAppSelector(mode) {
  appSelectorMode = mode;
  const titleEl = document.getElementById('app-selector-title');
  if (titleEl) {
    titleEl.textContent = mode === 'hma' ? 'Select Excluded Apps' : 'Select Target Apps';
  }
  
  document.getElementById('app-selector-modal').classList.remove('hidden');
  const container = document.getElementById('app-list-container');
  
  try {
    if (mode === 'hma') {
      const rules = await sh('hma.sh', 'get_rule', 'hma_exclude.txt');
      selectedAppsSet = new Set(rules.split('\n').map(s => s.trim()).filter(s => s));
    } else {
      const raw = await sh('teesimulator.sh', 'get_targets');
      selectedAppsSet = new Set(raw.split('\n').map(s => s.trim()).filter(s => s));
    }
  } catch(e) {
    selectedAppsSet = new Set();
  }

  if (container) container.innerHTML = '<div style="text-align:center;padding:20px;color:var(--desc);">Loading apps...</div>';
  
  if (installedApps.length === 0) {
    let labelsMap = {};
    try {
      const labelsRaw = await sh('hma.sh', 'get_all_labels');
      labelsRaw.split('\n').forEach(line => {
        const parts = line.split('|');
        if (parts.length >= 2) labelsMap[parts[0].trim()] = parts[1].trim();
      });
    } catch(e) {}

    try {
      if (typeof ksu !== 'undefined' && typeof ksu.getPackagesInfo === 'function') {
        let pkgs = await ksu.getPackagesInfo();
        if (typeof pkgs === 'string') pkgs = JSON.parse(pkgs);
        installedApps = pkgs.filter(p => !p.packageName.startsWith('com.android.') && !p.packageName.startsWith('android'));
        installedApps.forEach(app => {
          if (labelsMap[app.packageName]) app.label = labelsMap[app.packageName];
        });
      } else {
        throw new Error('ksu.getPackagesInfo not found');
      }
    } catch(e) {
      try {
        const raw = await sh('hma.sh', 'get_packages');
        installedApps = raw.split('\n').map(s => s.trim()).filter(s => s).map(p => ({ 
          packageName: p, 
          label: labelsMap[p] || p 
        }));
      } catch(err) {
        toast('Failed to load apps: ' + err.message);
        installedApps = [];
      }
    }
  }
  
  renderAppList(installedApps);
}

function renderAppList(apps) {
  const container = document.getElementById('app-list-container');
  if (container) container.innerHTML = '';
  
  if (apps.length === 0) {
    if (container) container.innerHTML = '<div style="text-align:center;padding:20px;color:var(--desc);">No apps found</div>';
    return;
  }

  if (!appObserver) {
    appObserver = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          const item = entry.target;
          
          // Lazy load icon
          const img = item.querySelector('.lazy-icon');
          if (img && !img.src) {
            img.src = img.dataset.src;
          }
          
          appObserver.unobserve(item);
        }
      });
    }, { rootMargin: '50px' });
  }

  const html = apps.map(app => {
    const isChecked = selectedAppsSet.has(app.packageName) ? 'checked' : '';
    const fallbackIcon = 'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZmlsbD0iI2NjYyIgZD0iTTEyIDJDMiAyIDIgMTIgMiAxMnMxMCAxMCAxMCAxMCAxMC0xMCAxMC0xMC0xMC0xMC0xMHptMCAxOGMtNC40MSAwLTgtMy41OS04LThzMy41OS04IDgtOCA4IDMuNTkgOCA4LTMuNTkgOC04IDh6Ii8+PC9zdmc+';
    const iconSrc = window.ksu ? `ksu://icon/${app.packageName}` : fallbackIcon;
    
    let resolvedLabel = app.label || app.appName || app.name || app.packageName;
    const isPackageName = (resolvedLabel === app.packageName);
    
    return `
      <div class="app-item lazy-item" onclick="toggleAppSelection('${app.packageName}', this)">
        <img class="app-icon lazy-icon" data-src="${iconSrc}" onerror="this.src='${fallbackIcon}'" />
        <div class="app-info">
          <div class="app-name">${resolvedLabel}</div>
          <div class="app-pkg" style="display: ${isPackageName ? 'none' : 'block'};">${app.packageName}</div>
        </div>
        <div class="app-checkbox ${isChecked}"></div>
      </div>
    `;
  }).join('');
  
  if (container) container.innerHTML = html;
  if (container) container.querySelectorAll('.lazy-item').forEach(item => appObserver.observe(item));
}

window.toggleAppSelection = async function(pkgName, elem) {
  const checkbox = elem.querySelector('.app-checkbox');
  if (selectedAppsSet.has(pkgName)) {
    selectedAppsSet.delete(pkgName);
    checkbox.classList.remove('checked');
  } else {
    selectedAppsSet.add(pkgName);
    checkbox.classList.add('checked');
  }
  
  const val = Array.from(selectedAppsSet).join('\n');
  try {
    if (appSelectorMode === 'hma') {
      await sh('hma.sh', 'set_rule', 'hma_exclude.txt', val);
    } else {
      const args = Array.from(selectedAppsSet);
      await sh('teesimulator.sh', 'set_targets', ...args);
    }
  } catch(e) {
    toast('Error saving app selection: ' + e.message);
  }
}

document.getElementById('btn-open-app-selector')?.addEventListener('click', () => openAppSelector('hma'));
document.getElementById('teesim-target-apps-btn')?.addEventListener('click', () => openAppSelector('teesim'));
document.getElementById('btn-close-app-selector')?.addEventListener('click', () => {
  document.getElementById('app-selector-modal').classList.add('hidden');
});
document.getElementById('app-search-input')?.addEventListener('input', (e) => {
  const q = e.target.value.toLowerCase();
  const filtered = installedApps.filter(app => 
    (app.label && app.label.toLowerCase().includes(q)) || 
    (app.packageName && app.packageName.toLowerCase().includes(q))
  );
  renderAppList(filtered);
});
