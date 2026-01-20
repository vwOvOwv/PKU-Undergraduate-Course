import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
BASE_URL = f"https://cdn.jsdelivr.net/gh/{USER}/{REPO}"

# 全局计数器，确保 ID 绝对唯一且前端友好
id_counter = 0

def get_next_id():
    global id_counter
    id_counter += 1
    return f"n{id_counter}"

def build_tree(path):
    """递归构建文件树"""
    tree = {"name": os.path.basename(path), "type": "folder", "children": []}
    
    if not os.path.exists(path):
        print(f"⚠️ Warning: Path not found: {path}")
        return tree
        
    try:
        items = os.listdir(path)
    except OSError as e:
        print(f"⚠️ Error reading {path}: {e}")
        return tree

    # 排序：文件夹在前，文件名忽略大小写排序
    items.sort(key=lambda x: (not os.path.isdir(os.path.join(path, x)), x.lower()))

    for item in items:
        # 过滤规则
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME", "README.md", "__pycache__"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            child_tree = build_tree(full_path)
            tree["children"].append(child_tree)
        else:
            tree["children"].append({
                "name": item,
                "type": "file",
                "path": full_path, 
                "size": round(os.path.getsize(full_path) / 1024, 2)
            })
    return tree

def process_branch_root(local_path, branch_name):
    """处理根目录，生成最终的前端数据结构"""
    print(f"Processing {local_path}...")
    raw_tree = build_tree(local_path)
    
    # 检查是否为空，如果为空可能是 checkout 失败
    if not raw_tree["children"]:
        print(f"⚠️ WARNING: No files found in {local_path}!")
        return []

    # 递归处理节点：生成ID，计算URL
    def process_node(node):
        node["id"] = get_next_id()
        
        if node["type"] == "file":
            rel_path = os.path.relpath(node["path"], local_path)
            # 统一路径分隔符
            safe_rel_path = "/".join(rel_path.split(os.sep))
            # 暂存相对路径供前端搜索显示用
            node["relPath"] = safe_rel_path 
            node["urlPath"] = f"{branch_name}/{safe_rel_path}"
            del node["path"]
        else:
            for child in node["children"]:
                process_node(child)

    process_node(raw_tree)
    return raw_tree["children"]

# 执行扫描
data_cs = process_branch_root("docs/computer-science", "ComputerScience")
data_ls = process_branch_root("docs/life-science", "LifeScience")

full_data = [
    {"name": "Computer Science", "type": "folder", "children": data_cs, "id": "root_cs"},
    {"name": "Life Science", "type": "folder", "children": data_ls, "id": "root_ls"}
]

# HTML 模板
html_template = """
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Peiyu's Course Zoo</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.10.1/jszip.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/FileSaver.js/2.0.5/FileSaver.min.js"></script>
    <style>
        :root { --primary: #0366d6; --bg: #f6f8fa; --border: #e1e4e8; --hover: #f1f8ff; --selected: #e1ecf4; }
        * { box-sizing: border-box; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; margin: 0; color: #24292e; display: flex; height: 100vh; overflow: hidden; background: #fff; }
        
        /* === 布局 === */
        .sidebar { width: 450px; border-right: 1px solid var(--border); background: #fcfcfc; display: flex; flex-direction: column; flex-shrink: 0; }
        .main-view { flex-grow: 1; display: flex; flex-direction: column; background: #fff; min-width: 0; }

        /* === 顶部栏 === */
        /* 1. 这是右侧顶部栏 (你之前不小心删掉的) */
        .header { height: 60px; border-bottom: 1px solid var(--border); display: flex; align-items: center; padding: 0 20px; justify-content: space-between; }
        .header h1 { font-size: 18px; margin: 0; white-space: nowrap; font-weight: bold; }
        .header-right { display: flex; align-items: center; gap: 15px; }
        .search-wrapper { position: relative; width: 250px; }

        /* 2. 这是左侧顶部栏 (与右侧对齐) */
        .sidebar-title { 
            height: 60px; /* 关键：与右侧 .header 高度一致 */
            display: flex; 
            align-items: center; 
            padding: 0 20px; 
            font-size: 18px; /* 字体大小与右侧 h1 保持一致 */
            font-weight: bold; 
            color: #24292e; 
            border-bottom: 1px solid var(--border); 
            background: #fcfcfc;
            flex-shrink: 0;
        }
        
        .search-wrapper { flex-grow: 1; max-width: 400px; position: relative; }
        .search-box { width: 100%; padding: 8px 12px 8px 30px; border: 1px solid var(--border); border-radius: 6px; font-size: 14px; background: #f6f8fa; transition: .2s; }
        .search-box:focus { background: #fff; border-color: var(--primary); outline: none; box-shadow: 0 0 0 3px rgba(3,102,214,0.1); }
        .search-icon { position: absolute; left: 8px; top: 50%; transform: translateY(-50%); color: #999; font-size: 14px; pointer-events: none; }

        .btn { background-color: #2ea44f; color: white; border: 1px solid rgba(27,31,35,.15); padding: 6px 16px; font-size: 14px; font-weight: 600; border-radius: 6px; cursor: pointer; display: flex; align-items: center; gap: 6px; white-space: nowrap; }
        .btn:disabled { background-color: #94d3a2; cursor: not-allowed; opacity: 0.7; }
        .btn:hover:not(:disabled) { background-color: #2c974b; }

        /* === 侧边栏树 === */
        .tree-container { flex-grow: 1; overflow-y: auto; overflow-x: auto; padding: 10px 0; }
        .tree-item { padding: 6px 15px 6px 10px; cursor: pointer; white-space: nowrap; display: flex; align-items: center; font-size: 14px; color: #444; user-select: none; }
        .tree-item:hover { background-color: var(--hover); }
        .tree-item.active { background-color: var(--selected); color: var(--primary); font-weight: 500; }
        .tree-toggle { width: 20px; text-align: center; color: #999; font-size: 10px; transition: transform 0.15s; margin-right: 4px; display: inline-flex; justify-content: center; }
        .tree-toggle.open { transform: rotate(90deg); }
        .tree-toggle.invisible { visibility: hidden; }
        .tree-icon { margin-right: 6px; color: #54aeff; }

        /* === 主视图表格 === */
        .file-list { flex-grow: 1; overflow-y: auto; }
        table { width: 100%; border-collapse: collapse; table-layout: fixed; }
        th { text-align: left; padding: 10px 15px; border-bottom: 1px solid var(--border); background: #fafbfc; color: #586069; font-size: 13px; font-weight: 600; position: sticky; top: 0; z-index: 10; }
        td { padding: 8px 15px; border-bottom: 1px solid #eaecef; font-size: 14px; vertical-align: middle; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; color: #24292e; }
        tr { cursor: default; }
        tr:hover { background-color: var(--hover); }
        tr.selected { background-color: var(--selected); }

        .col-check { width: 40px; text-align: center; }
        .col-icon { width: 36px; text-align: center; font-size: 16px; }
        .col-name { width: auto; }
        .col-size { width: auto; text-align: right;}
        
        .path-hint { display: block; font-size: 11px; color: #888; margin-top: 2px; }
        .match-highlight { background-color: #fff5b1; border-radius: 2px; }

        /* 面包屑 */
        .breadcrumbs { padding: 8px 20px; font-size: 13px; color: #586069; border-bottom: 1px solid var(--border); display: flex; align-items: center; gap: 6px; overflow-x: auto; white-space: nowrap; background: #fff; min-height: 36px; }
        .crumb { cursor: pointer; color: var(--primary); }
        .crumb:hover { text-decoration: underline; }
        .crumb.current { color: #24292e; font-weight: 600; pointer-events: none; }
        .crumb-sep { color: #ccc; font-size: 12px; }

        /* 加载与空状态 */
        .empty-state { padding: 50px; text-align: center; color: #586069; }
        #loading-overlay { position: fixed; top: 0; left: 0; right: 0; bottom: 0; background: rgba(255,255,255,0.8); display: flex; flex-direction: column; align-items: center; justify-content: center; z-index: 100; display: none; }
        .spinner { width: 30px; height: 30px; border: 3px solid #eee; border-top: 3px solid var(--primary); border-radius: 50%; animation: spin 0.8s linear infinite; }
        @keyframes spin { to { transform: rotate(360deg); } }

        /* 移动端适配 */
        @media (max-width: 768px) {
            .sidebar { display: none; }
            .col-size { display: none; }
            .header-left { gap: 10px; }
            .search-wrapper { max-width: 150px; }
        }
    </style>
</head>
<body>
    <div class="sidebar">
        <div class="sidebar-title">Peiyu's Course Zoo</div>
        <div class="tree-container" id="folder-tree"></div>
    </div>

    <div class="main-view">
        <div class="header">
            <h1 id="view-title">Course Zoo</h1>
            
            <div class="header-right">
                <div class="search-wrapper">
                    <span class="search-icon">🔍</span>
                    <input type="text" class="search-box" id="search-input" placeholder="Search files..." oninput="handleSearch(this.value)">
                </div>
                <button class="btn" id="download-btn" onclick="downloadSelected()" disabled>
                    <span>⬇</span> Download
                </button>
            </div>
        </div>
        
        <div class="breadcrumbs" id="breadcrumbs"></div>

        <div class="file-list">
            <table>
                <thead>
                    <tr>
                        <th class="col-check"><input type="checkbox" id="select-all" onclick="toggleSelectAll()"></th>
                        <th class="col-icon"></th>
                        <th class="col-name">Name</th>
                        <th class="col-size">Size</th>
                    </tr>
                </thead>
                <tbody id="file-table-body"></tbody>
            </table>
            <div id="empty-msg" class="empty-state" style="display:none;">This folder is empty.</div>
        </div>
    </div>

    <div id="loading-overlay">
        <div class="spinner"></div>
        <div id="loading-text" style="margin-top: 15px; font-size: 14px; font-weight: 500;">Processing...</div>
    </div>

    <script>
        const rawData = __DATA_JSON__;
        const BASE_URL = "__BASE_URL__";
        
        let currentFolder = null;
        let isSearchMode = false;
        let folderMap = new Map();
        let allFiles = []; // 搜索索引
        let selectedFiles = new Set();

        // === 1. 初始化索引 (修改版：同时索引文件夹) ===
        let allFiles = []; // 搜索索引（包含文件和文件夹）

        function indexData(nodes, parentPath = []) {
            if (!nodes) return;
            nodes.forEach(node => {
                node.parent = parentPath;
                
                // 1. 存入 Map 供 ID 查找
                if (node.type === 'folder') {
                    if (node.id) folderMap.set(node.id, node);
                    // 递归处理子节点
                    if (node.children) indexData(node.children, [...parentPath, node]);
                } 
                
                // 2. 存入搜索索引 (文件和文件夹都加进去)
                // 此时我们要为文件夹动态生成一个 path 字符串，方便搜索显示
                if (!node.relPath && node.parent) {
                    node.relPath = node.parent.map(p => p.name).join('/') + '/' + node.name;
                }
                allFiles.push(node);
            });
        }
        
        // 简单防御：检查数据是否为空
        if (!rawData || rawData.length === 0 || (rawData.length === 2 && !rawData[0].children && !rawData[1].children)) {
            document.getElementById('file-table-body').innerHTML = '<tr><td colspan="4" class="empty-state">⚠️ Error: No course data found.<br>Please check build logs.</td></tr>';
            console.error("Raw Data is empty or invalid", rawData);
        } else {
            indexData(rawData);
            renderTree(rawData, document.getElementById('folder-tree'));
            // 默认打开第一个根目录
            if(rawData[0]) openFolder(rawData[0].id);
        }

        // === 2. 渲染左侧树 ===
        function renderTree(nodes, container, level = 0) {
            nodes.forEach(node => {
                if (node.type !== 'folder') return;

                const div = document.createElement('div');
                div.className = 'tree-item';
                div.dataset.id = node.id;
                div.style.paddingLeft = '15px';
                div.onclick = (e) => { e.stopPropagation(); openFolder(node.id); };

                const showChildren = level < 1; 
                const hasSubs = showChildren && node.children && node.children.some(c => c.type === 'folder');
                
                const toggle = document.createElement('span');
                toggle.className = 'tree-toggle ' + (hasSubs ? '' : 'invisible');
                toggle.innerText = '▶';
                
                // const icon = document.createElement('span');
                // icon.className = 'tree-icon';
                // icon.innerText = '📁';

                const name = document.createElement('span');
                name.innerText = node.name;

                if (level === 0) {
                    name.style.fontWeight = 'bold';
                }

                div.append(toggle, name);
                container.appendChild(div);

                if (hasSubs) {
                    const subDiv = document.createElement('div');
                    subDiv.style.display = 'none';
                    renderTree(node.children, subDiv, level + 1);
                    container.appendChild(subDiv);
                    
                    toggle.onclick = (e) => {
                        e.stopPropagation();
                        const isOpen = subDiv.style.display === 'block';
                        subDiv.style.display = isOpen ? 'none' : 'block';
                        toggle.classList.toggle('open', !isOpen);
                    };
                }
            });
        }

        // === 3. 渲染右侧列表 (修改版：支持搜索跳转) ===
        function renderList(items, isSearch = false) {
            const tbody = document.getElementById('file-table-body');
            tbody.innerHTML = '';
            document.getElementById('empty-msg').style.display = items.length ? 'none' : 'block';
            document.getElementById('select-all').checked = false;
            
            // 更新标题和面包屑
            if (isSearch) {
                document.getElementById('view-title').innerText = `Search Results (${items.length})`;
                document.getElementById('breadcrumbs').innerHTML = '<span class="crumb" onclick="exitSearch()">Back to Folders</span> <span class="crumb-sep">/</span> <span class="crumb current">Search</span>';
            } else {
                document.getElementById('view-title').innerText = currentFolder ? currentFolder.name : 'Root';
                updateBreadcrumbs(currentFolder);
            }

            // 排序: 文件夹在前
            if (!isSearch) {
                items.sort((a, b) => {
                    if (a.type === b.type) return a.name.localeCompare(b.name);
                    return a.type === 'folder' ? -1 : 1;
                });
            }

            items.forEach(item => {
                const tr = document.createElement('tr');
                
                // --- 行为逻辑：点击跳转 ---
                // 定义点击行的行为
                const handleRowClick = () => {
                    if (isSearch) {
                        // 搜索模式下：点击即跳转
                        jumpTo(item);
                    } else {
                        // 普通模式下：文件夹进入，文件无动作(或选框)
                        if (item.type === 'folder') openFolder(item.id);
                    }
                };

                // 1. Checkbox 列
                const tdCheck = document.createElement('td');
                tdCheck.className = 'col-check';
                const cb = document.createElement('input');
                cb.type = 'checkbox';
                
                if (item.type === 'file') {
                    cb.checked = selectedFiles.has(item);
                    cb.onchange = (e) => toggleSelection(item, e.target.checked);
                    // 点击行时，如果是搜索模式则跳转，否则切换选中
                    tr.onclick = (e) => { 
                        if (e.target !== cb) {
                            if (isSearch) jumpTo(item); 
                            else cb.click(); 
                        }
                    };
                    if (cb.checked) tr.classList.add('selected');
                } else {
                    // 文件夹没有 Checkbox，点击行只触发跳转/进入
                    cb.disabled = true;
                    cb.style.opacity = 0.3;
                    tr.onclick = handleRowClick;
                }
                tdCheck.appendChild(cb);

                // 2. Icon 列
                const tdIcon = document.createElement('td');
                tdIcon.className = 'col-icon';
                tdIcon.innerText = item.type === 'folder' ? '📁' : '📄';

                // 3. Name 列
                const tdName = document.createElement('td');
                tdName.className = 'col-name';
                
                if (isSearch) {
                    // 搜索模式：显示路径提示
                    // 移除开头多余的斜杠或根路径显示，只保留父级路径
                    const parentPath = item.parent ? item.parent.map(p => p.name).join('/') : '';
                    tdName.innerHTML = `<div>${item.name}</div><span class="path-hint">${parentPath}</span>`;
                } else {
                    tdName.innerText = item.name;
                }

                // 4. Size 列
                const tdSize = document.createElement('td');
                tdSize.className = 'col-size';
                tdSize.innerText = item.type === 'folder' ? '-' : item.size + ' KB';

                tr.append(tdCheck, tdIcon, tdName, tdSize);
                tbody.appendChild(tr);
            });
            
            updateBtnState();
        }

        // === 新增：跳转逻辑 ===
        function jumpTo(item) {
            // 1. 清空搜索状态
            document.getElementById('search-input').value = '';
            isSearchMode = false;

            // 2. 决定要打开哪个文件夹 ID
            let targetFolderId = null;

            if (item.type === 'folder') {
                // 如果搜索到的是文件夹，直接打开它
                targetFolderId = item.id;
            } else {
                // 如果搜索到的是文件，打开它的父文件夹
                if (item.parent && item.parent.length > 0) {
                    targetFolderId = item.parent[item.parent.length - 1].id;
                } else {
                    // 极其罕见的根目录文件情况
                    if (rawData[0]) targetFolderId = rawData[0].id;
                }
            }

            // 3. 执行打开
            if (targetFolderId) {
                openFolder(targetFolderId);
                
                // 可选：如果是文件，可以加一点高亮效果（这里简单实现为滚动到顶部）
                // 实际生产中可能需要用 ID 锚点定位
                document.querySelector('.file-list').scrollTop = 0; 
            }
        }

        function openFolder(id) {
            isSearchMode = false;
            document.getElementById('search-input').value = ''; // 清空搜索
            const node = folderMap.get(id);
            if (!node) return;
            currentFolder = node;
            
            // 左侧高亮
            document.querySelectorAll('.tree-item').forEach(el => el.classList.remove('active'));
            const activeItem = document.querySelector(`.tree-item[data-id="${id}"]`);
            if (activeItem) activeItem.classList.add('active');

            renderList(node.children || []);
        }

        // === 4. 搜索功能 ===
        function handleSearch(val) {
            val = val.trim().toLowerCase();
            if (!val) {
                if (isSearchMode && currentFolder) openFolder(currentFolder.id);
                return;
            }
            
            isSearchMode = true;
            // 搜索算法：匹配文件名
            const results = allFiles.filter(f => f.name.toLowerCase().includes(val));
            renderList(results, true);
        }

        function exitSearch() {
            document.getElementById('search-input').value = '';
            if (currentFolder) openFolder(currentFolder.id);
            else if (rawData[0]) openFolder(rawData[0].id);
        }

        // === 5. 其他逻辑 ===
        function updateBreadcrumbs(node) {
            const container = document.getElementById('breadcrumbs');
            container.innerHTML = '';
            if (Array.isArray(node) || !node) return;

            const path = [...(node.parent || []), node];
            path.forEach((crumb, index) => {
                if (index > 0) container.innerHTML += '<span class="crumb-sep">/</span>';
                const span = document.createElement('span');
                span.className = 'crumb' + (index === path.length - 1 ? ' current' : '');
                span.innerText = crumb.name;
                span.onclick = () => openFolder(crumb.id);
                container.appendChild(span);
            });
        }

        function toggleSelection(item, isChecked) {
            if (isChecked) selectedFiles.add(item);
            else selectedFiles.delete(item);
            
            // 更新当前列表的UI
            // (为了性能，这里不重新render整个列表，只更新class)
            // 实际上 renderList 很轻量，重新渲染也可以，或者遍历 DOM
            const checkboxes = document.querySelectorAll('input[type="checkbox"]');
            // ...简化处理，直接重新 render 可能会断开 focus，所以这里只更新按钮
            updateBtnState();
        }
        
        function toggleSelectAll() {
            const isChecked = document.getElementById('select-all').checked;
            const rows = document.querySelectorAll('#file-table-body tr');
            
            // 获取当前显示的列表数据
            let currentItems = isSearchMode ? 
                allFiles.filter(f => f.name.toLowerCase().includes(document.getElementById('search-input').value.toLowerCase())) 
                : (currentFolder ? currentFolder.children : []);

            currentItems.forEach(item => {
                if (item.type === 'file') {
                    if (isChecked) selectedFiles.add(item);
                    else selectedFiles.delete(item);
                }
            });
            renderList(currentItems, isSearchMode);
        }

        function updateBtnState() {
            const btn = document.getElementById('download-btn');
            const count = selectedFiles.size;
            btn.disabled = count === 0;
            btn.innerHTML = count > 0 ? `<span>⬇</span> Download (${count})` : `<span>⬇</span> Download`;
        }

        // === 6. 下载逻辑 ===
        async function downloadSelected() {
            if (selectedFiles.size === 0) return;
            
            const overlay = document.getElementById('loading-overlay');
            const text = document.getElementById('loading-text');
            overlay.style.display = 'flex';
            
            const zip = new JSZip();
            let count = 0;
            
            try {
                for (const file of selectedFiles) {
                    text.innerText = `Fetching ${count + 1}/${selectedFiles.size}: ${file.name}`;
                    
                    // URL Encode
                    const parts = file.urlPath.split('/');
                    const encodedPath = parts.map(p => encodeURIComponent(p)).join('/');
                    const url = `${BASE_URL}/${encodedPath}`;
                    
                    const res = await fetch(url);
                    if (!res.ok) throw new Error(`HTTP ${res.status}`);
                    zip.file(file.name, await res.blob());
                    count++;
                }
                
                text.innerText = "Zipping...";
                const content = await zip.generateAsync({type:"blob"});
                saveAs(content, "PKU_Materials.zip");
                
            } catch (err) {
                alert("Download failed: " + err.message);
            } finally {
                overlay.style.display = 'none';
                // selectedFiles.clear(); // 可选：下载后是否清空
                // updateBtnState();
            }
        }
    </script>
</body>
</html>
"""

# 生成 HTML
final_html = html_template.replace("__DATA_JSON__", json.dumps(full_data))
final_html = final_html.replace("__BASE_URL__", BASE_URL)

with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(final_html)

print("✅ Index generated successfully with Search & Safe IDs.")