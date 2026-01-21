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

def get_folder_size(path):
    """递归计算文件夹大小（KB）"""
    total = 0
    try:
        for entry in os.scandir(path):
            if entry.is_file():
                total += entry.stat().st_size
            elif entry.is_dir():
                total += get_folder_size(entry.path) * 1024  # 转回字节
    except OSError:
        pass
    return round(total / 1024, 2)  # 返回 KB

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

    folder_size = 0
    for item in items:
        # 过滤规则
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME", "README.md", "__pycache__"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            child_tree = build_tree(full_path)
            folder_size += child_tree.get("size", 0)
            tree["children"].append(child_tree)
        else:
            file_size = round(os.path.getsize(full_path) / 1024, 2)
            folder_size += file_size
            tree["children"].append({
                "name": item,
                "type": "file",
                "path": full_path, 
                "size": file_size
            })
    
    tree["size"] = round(folder_size, 2)
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

# 计算根目录大小
def calculate_folder_size(children):
    total = 0
    for item in children:
        total += item.get("size", 0)
    return round(total, 2)

full_data = [
    {"name": "Computer Science", "type": "folder", "children": data_cs, "id": "root_cs", "size": calculate_folder_size(data_cs)},
    {"name": "Life Science", "type": "folder", "children": data_ls, "id": "root_ls", "size": calculate_folder_size(data_ls)}
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
        td { padding: 8px 15px; border-bottom: 1px solid #eaecef; font-size: 14px; vertical-align: middle; white-space: nowrap; color: #24292e; }
        td.col-name { overflow: hidden; text-overflow: ellipsis; }
        tr { cursor: default; }
        tr:hover { background-color: var(--hover); }
        tr.selected { background-color: var(--selected); }

        .col-check { width: 40px; text-align: center; overflow: visible; }
        .col-icon { width: 36px; text-align: center; font-size: 16px; }
        .col-name { width: auto; }
        .col-size { width: auto; text-align: right;}
        
        .path-hint { display: block; font-size: 11px; color: #888; margin-top: 2px; }
        .path-hint .path-crumb { color: #0366d6; cursor: pointer; }
        .path-hint .path-crumb:hover { text-decoration: underline; }
        .match-highlight { background-color: #fff5b1; border-radius: 2px; }
        
        /* 搜索结果行样式 */
        tr.search-result { cursor: pointer; }
        tr.search-result:hover { background-color: #dbeafe; }
        tr.search-result td.col-name::after { content: ''; }
        tr.search-result:hover td.col-name::after { opacity: 1; }
        .jump-hint { font-size: 11px; color: #0366d6; margin-left: 8px; opacity: 0.7; }

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

        /* === Download Cart 样式 === */
        .download-cart {
            border-top: 1px solid var(--border);
            background: linear-gradient(180deg, #f8fafc 0%, #f1f5f9 100%);
            display: flex;
            flex-direction: column;
            max-height: 45%;
            min-height: 60px;
            transition: max-height 0.3s ease;
        }
        .download-cart.collapsed {
            max-height: 48px;
            min-height: 48px;
        }
        .cart-header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 12px 16px;
            cursor: pointer;
            user-select: none;
            background: #fff;
            border-bottom: 1px solid var(--border);
        }
        .cart-header:hover {
            background: var(--hover);
        }
        .cart-title {
            display: flex;
            align-items: center;
            gap: 8px;
            font-weight: 600;
            font-size: 14px;
            color: #24292e;
        }
        .cart-badge {
            background: var(--primary);
            color: white;
            font-size: 11px;
            font-weight: 600;
            padding: 2px 7px;
            border-radius: 10px;
            min-width: 20px;
            text-align: center;
        }
        .cart-badge.empty {
            background: #94a3b8;
        }
        .cart-toggle {
            font-size: 12px;
            color: #64748b;
            transition: transform 0.2s;
        }
        .download-cart.collapsed .cart-toggle {
            transform: rotate(180deg);
        }
        .cart-body {
            flex-grow: 1;
            overflow-y: auto;
            padding: 8px 0;
        }
        .download-cart.collapsed .cart-body,
        .download-cart.collapsed .cart-footer {
            display: none;
        }
        .cart-item {
            display: flex;
            align-items: center;
            padding: 8px 16px;
            gap: 10px;
            font-size: 13px;
            color: #334155;
            transition: background 0.15s;
        }
        .cart-item:hover {
            background: var(--hover);
        }
        .cart-item-icon {
            flex-shrink: 0;
        }
        .cart-item-name {
            flex-grow: 1;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
        }
        .cart-item-size {
            font-size: 11px;
            color: #94a3b8;
            flex-shrink: 0;
        }
        .cart-item-remove {
            flex-shrink: 0;
            width: 20px;
            height: 20px;
            border: none;
            background: transparent;
            color: #94a3b8;
            cursor: pointer;
            border-radius: 4px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 14px;
            transition: all 0.15s;
        }
        .cart-item-remove:hover {
            background: #fee2e2;
            color: #ef4444;
        }
        .cart-empty {
            text-align: center;
            padding: 20px;
            color: #94a3b8;
            font-size: 13px;
        }
        .cart-footer {
            padding: 12px 16px;
            border-top: 1px solid var(--border);
            background: #fff;
            display: flex;
            flex-direction: column;
            gap: 10px;
        }
        .cart-stats {
            font-size: 12px;
            color: #64748b;
            display: flex;
            justify-content: space-between;
        }
        .cart-actions {
            display: flex;
            gap: 8px;
        }
        .btn-cart {
            flex: 1;
            padding: 8px 12px;
            border-radius: 6px;
            font-size: 13px;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.15s;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
        }
        .btn-cart-clear {
            background: #f1f5f9;
            border: 1px solid #e2e8f0;
            color: #64748b;
        }
        .btn-cart-clear:hover {
            background: #fee2e2;
            border-color: #fecaca;
            color: #dc2626;
        }
        .btn-cart-download {
            background: linear-gradient(135deg, #2ea44f 0%, #22863a 100%);
            border: none;
            color: white;
        }
        .btn-cart-download:hover {
            background: linear-gradient(135deg, #22863a 0%, #176f2c 100%);
            box-shadow: 0 2px 8px rgba(34, 134, 58, 0.3);
        }
        .btn-cart-download:disabled {
            background: #94d3a2;
            cursor: not-allowed;
            box-shadow: none;
        }
        .btn-add-cart {
            background: #dbeafe;
            color: #1d4ed8;
            border: 1px solid #bfdbfe;
        }
        .btn-add-cart:hover:not(:disabled) {
            background: #bfdbfe;
        }
        .btn-add-cart:disabled {
            background: #f1f5f9;
            color: #94a3b8;
            border-color: #e2e8f0;
            cursor: not-allowed;
        }

        /* 动画 */
        @keyframes cartItemAdd {
            from { opacity: 0; transform: translateX(-10px); }
            to { opacity: 1; transform: translateX(0); }
        }
        .cart-item {
            animation: cartItemAdd 0.2s ease;
        }
    </style>
</head>
<body>
    <div class="sidebar">
        <div class="sidebar-title">Peiyu's Course Zoo</div>
        <div class="tree-container" id="folder-tree"></div>
        
        <!-- Download Cart Panel -->
        <div class="download-cart" id="download-cart">
            <div class="cart-header" onclick="toggleCartPanel()">
                <div class="cart-title">
                    <span>🛒</span>
                    <span>Download Cart</span>
                    <span class="cart-badge empty" id="cart-badge">0</span>
                </div>
                <span class="cart-toggle">▼</span>
            </div>
            <div class="cart-body" id="cart-body">
                <div class="cart-empty" id="cart-empty">No items in cart</div>
            </div>
            <div class="cart-footer">
                <div class="cart-stats">
                    <span id="cart-file-count">0 files</span>
                    <span id="cart-total-size">~0 KB</span>
                </div>
                <div class="cart-actions">
                    <button class="btn-cart btn-cart-clear" onclick="clearCart()">🗑 Clear All</button>
                    <button class="btn-cart btn-cart-download" id="cart-download-btn" onclick="downloadCart()" disabled>⬇ Download All</button>
                </div>
            </div>
        </div>
    </div>

    <div class="main-view">
        <div class="header">
            <h1 id="view-title">Course Zoo</h1>
            
            <div class="header-right">
                <div class="search-wrapper">
                    <span class="search-icon">🔍</span>
                    <input type="text" class="search-box" id="search-input" placeholder="Search folders or files..." oninput="handleSearch(this.value)">
                </div>
                <button class="btn btn-add-cart" id="add-cart-btn" onclick="addToCart()" disabled>
                    <span>🛒</span> Add to Cart
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
        let cartItems = new Map(); // Download Cart storage

        // === 辅助函数：格式化文件大小 ===
        function formatSize(sizeKB) {
            if (sizeKB == null || sizeKB === 0) return '-';
            if (sizeKB >= 1024) {
                return (sizeKB / 1024).toFixed(1) + ' MB';
            }
            return sizeKB.toFixed(1) + ' KB';
        }

        // === 1. 初始化索引 (修改版：同时索引文件夹) ===

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
                let mainTitle = 'Course Zoo'; // 默认兜底
                if (currentFolder) {
                    // 逻辑：如果当前文件夹有父级列表且不为空，取第一个父级（即根节点）；
                    // 否则，说明当前文件夹本身就是根节点 (如 Computer Science)
                    const rootNode = (currentFolder.parent && currentFolder.parent.length > 0) 
                                     ? currentFolder.parent[0] 
                                     : currentFolder;
                    mainTitle = rootNode.name;
                }
                document.getElementById('view-title').innerText = mainTitle;
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
                if (isSearch) tr.classList.add('search-result');
                
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
                
                // 文件和文件夹都可以被选中
                cb.checked = selectedFiles.has(item);
                cb.onchange = (e) => {
                    e.stopPropagation();
                    toggleSelection(item, e.target.checked);
                };
                cb.onclick = (e) => e.stopPropagation();
                
                // 点击行的行为
                tr.onclick = (e) => { 
                    if (e.target !== cb) {
                        if (isSearch) {
                            jumpTo(item);
                        } else if (item.type === 'folder') {
                            // 文件夹：双击进入，单击选中
                            openFolder(item.id);
                        } else {
                            // 文件：点击切换选中
                            cb.click();
                        }
                    }
                };
                
                if (cb.checked) tr.classList.add('selected');
                tdCheck.appendChild(cb);

                // 2. Icon 列
                const tdIcon = document.createElement('td');
                tdIcon.className = 'col-icon';
                tdIcon.innerText = item.type === 'folder' ? '📁' : '📄';

                // 3. Name 列
                const tdName = document.createElement('td');
                tdName.className = 'col-name';
                
                if (isSearch) {
                    // 搜索模式：显示可点击的路径面包屑
                    const searchVal = document.getElementById('search-input').value.toLowerCase();
                    
                    // 高亮匹配的关键词
                    let displayName = item.name;
                    if (searchVal) {
                        const regex = new RegExp(`(${searchVal.replace(/[.*+?^${}()|[\\]\\\\]/g, '\\\\$&')})`, 'gi');
                        displayName = item.name.replace(regex, '<span class="match-highlight">$1</span>');
                    }
                    
                    // 构建可点击的路径面包屑
                    let pathHtml = '';
                    if (item.parent && item.parent.length > 0) {
                        const crumbs = item.parent.map((p, idx) => 
                            `<span class="path-crumb" data-folder-id="${p.id}">${p.name}</span>`
                        ).join(' / ');
                        pathHtml = `<span class="path-hint">${crumbs}</span>`;
                    }
                    
                    tdName.innerHTML = `<div>${displayName}<span class="jump-hint">Go to folder</span></div>${pathHtml}`;
                    
                    // 为路径面包屑添加点击事件
                    setTimeout(() => {
                        tdName.querySelectorAll('.path-crumb').forEach(crumb => {
                            crumb.onclick = (e) => {
                                e.stopPropagation();
                                const folderId = crumb.dataset.folderId;
                                if (folderId) {
                                    document.getElementById('search-input').value = '';
                                    isSearchMode = false;
                                    openFolder(folderId);
                                }
                            };
                        });
                    }, 0);
                } else {
                    tdName.innerText = item.name;
                }

                // 4. Size 列
                const tdSize = document.createElement('td');
                tdSize.className = 'col-size';
                tdSize.innerText = formatSize(item.size);

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
                // 文件和文件夹都可以被选中
                if (isChecked) selectedFiles.add(item);
                else selectedFiles.delete(item);
            });
            renderList(currentItems, isSearchMode);
        }

        function updateBtnState() {
            const addCartBtn = document.getElementById('add-cart-btn');
            const count = selectedFiles.size;
            addCartBtn.disabled = count === 0;
            addCartBtn.innerHTML = count > 0 ? `<span>🛒</span> Add to Cart (${count})` : `<span>🛒</span> Add to Cart`;
        }

        // === Download Cart Functions ===
        
        function toggleCartPanel() {
            const cart = document.getElementById('download-cart');
            cart.classList.toggle('collapsed');
        }

        function addToCart() {
            if (selectedFiles.size === 0) return;
            
            selectedFiles.forEach(item => {
                if (!cartItems.has(item.id)) {
                    cartItems.set(item.id, item);
                }
            });
            
            // Clear selection after adding
            selectedFiles.clear();
            if (currentFolder) {
                renderList(currentFolder.children || [], false);
            }
            
            updateCartUI();
            saveCartToStorage();
            
            // Expand cart panel if collapsed
            const cart = document.getElementById('download-cart');
            if (cart.classList.contains('collapsed')) {
                cart.classList.remove('collapsed');
            }
        }

        function removeFromCart(itemId) {
            cartItems.delete(itemId);
            updateCartUI();
            saveCartToStorage();
        }

        function clearCart() {
            if (cartItems.size === 0) return;
            if (!confirm('Clear all items from the cart?')) return;
            cartItems.clear();
            updateCartUI();
            saveCartToStorage();
        }

        function getAllFilesRecursive(node) {
            let files = [];
            if (node.type === 'file') {
                files.push(node);
            } else if (node.type === 'folder' && node.children) {
                node.children.forEach(child => {
                    files = files.concat(getAllFilesRecursive(child));
                });
            }
            return files;
        }

        function calculateTotalSize() {
            let totalKB = 0;
            cartItems.forEach(item => {
                if (item.type === 'file') {
                    totalKB += item.size || 0;
                } else {
                    // Recursively calculate folder size
                    const files = getAllFilesRecursive(item);
                    files.forEach(f => totalKB += f.size || 0);
                }
            });
            return totalKB;
        }

        function countFiles() {
            let count = 0;
            cartItems.forEach(item => {
                if (item.type === 'file') {
                    count++;
                } else {
                    count += getAllFilesRecursive(item).length;
                }
            });
            return count;
        }

        function updateCartUI() {
            const body = document.getElementById('cart-body');
            const badge = document.getElementById('cart-badge');
            const emptyMsg = document.getElementById('cart-empty');
            const downloadBtn = document.getElementById('cart-download-btn');
            const fileCountEl = document.getElementById('cart-file-count');
            const totalSizeEl = document.getElementById('cart-total-size');
            
            // Update badge
            const itemCount = cartItems.size;
            badge.textContent = itemCount;
            badge.classList.toggle('empty', itemCount === 0);
            
            // Update stats
            const fileCount = countFiles();
            const totalSize = calculateTotalSize();
            fileCountEl.textContent = fileCount === 1 ? '1 file' : `${fileCount} files`;
            totalSizeEl.textContent = totalSize >= 1024 ? `~${(totalSize / 1024).toFixed(1)} MB` : `~${totalSize.toFixed(0)} KB`;
            
            // Update download button
            downloadBtn.disabled = itemCount === 0;
            
            // Clear and rebuild body
            body.innerHTML = '';
            
            if (itemCount === 0) {
                body.innerHTML = '<div class="cart-empty">No items in cart</div>';
                return;
            }
            
            cartItems.forEach((item, id) => {
                const div = document.createElement('div');
                div.className = 'cart-item';
                
                const icon = document.createElement('span');
                icon.className = 'cart-item-icon';
                icon.textContent = item.type === 'folder' ? '📁' : '📄';
                
                const name = document.createElement('span');
                name.className = 'cart-item-name';
                name.textContent = item.name;
                name.title = item.name;
                
                const size = document.createElement('span');
                size.className = 'cart-item-size';
                if (item.type === 'file') {
                    size.textContent = item.size + ' KB';
                } else {
                    const folderFiles = getAllFilesRecursive(item);
                    size.textContent = `${folderFiles.length} files`;
                }
                
                const removeBtn = document.createElement('button');
                removeBtn.className = 'cart-item-remove';
                removeBtn.textContent = '✕';
                removeBtn.title = 'Remove from cart';
                removeBtn.onclick = (e) => {
                    e.stopPropagation();
                    removeFromCart(id);
                };
                
                div.append(icon, name, size, removeBtn);
                body.appendChild(div);
            });
        }

        async function downloadCart() {
            if (cartItems.size === 0) return;
            
            const overlay = document.getElementById('loading-overlay');
            const text = document.getElementById('loading-text');
            overlay.style.display = 'flex';
            
            // Collect all files (flatten folders)
            let allCartFiles = [];
            cartItems.forEach(item => {
                if (item.type === 'file') {
                    allCartFiles.push({ file: item, path: item.name });
                } else {
                    // For folders, preserve directory structure
                    const files = getAllFilesRecursive(item);
                    files.forEach(f => {
                        // Build relative path within the folder
                        const folderPath = item.name;
                        const filePath = f.relPath.replace(item.relPath + '/', '').replace(item.relPath, '');
                        allCartFiles.push({ file: f, path: folderPath + '/' + (filePath || f.name) });
                    });
                }
            });
            
            const zip = new JSZip();
            let count = 0;
            
            try {
                for (const {file, path} of allCartFiles) {
                    text.innerText = `Fetching ${count + 1}/${allCartFiles.length}: ${file.name}`;
                    
                    const parts = file.urlPath.split('/');
                    const encodedPath = parts.map(p => encodeURIComponent(p)).join('/');
                    const url = `${BASE_URL}/${encodedPath}`;
                    
                    const res = await fetch(url);
                    if (!res.ok) throw new Error(`HTTP ${res.status} for ${file.name}`);
                    zip.file(path, await res.blob());
                    count++;
                }
                
                text.innerText = 'Zipping...';
                const content = await zip.generateAsync({type:'blob'});
                saveAs(content, 'PKU_Materials.zip');
                
            } catch (err) {
                alert('Download failed: ' + err.message);
            } finally {
                overlay.style.display = 'none';
            }
        }

        // === LocalStorage Persistence ===
        
        function saveCartToStorage() {
            try {
                const ids = Array.from(cartItems.keys());
                localStorage.setItem('downloadCart', JSON.stringify(ids));
            } catch (e) {
                console.warn('Failed to save cart to localStorage:', e);
            }
        }

        function loadCartFromStorage() {
            try {
                const saved = localStorage.getItem('downloadCart');
                if (!saved) return;
                
                const ids = JSON.parse(saved);
                ids.forEach(id => {
                    // Find item by ID in allFiles
                    const item = allFiles.find(f => f.id === id);
                    if (item) {
                        cartItems.set(id, item);
                    }
                });
                updateCartUI();
            } catch (e) {
                console.warn('Failed to load cart from localStorage:', e);
            }
        }

        // Load cart on page load
        loadCartFromStorage();

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