import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
# 使用 jsDelivr CDN
BASE_URL = f"https://cdn.jsdelivr.net/gh/{USER}/{REPO}"

def build_tree(path):
    """递归构建文件树"""
    tree = {"name": os.path.basename(path), "type": "folder", "children": []}
    if not os.path.exists(path):
        return tree
        
    try:
        items = os.listdir(path)
    except OSError:
        return tree

    # 排序：文件夹在前
    items.sort(key=lambda x: (not os.path.isdir(os.path.join(path, x)), x))

    for item in items:
        # 过滤系统文件
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME", "README.md", "__pycache__"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            child_tree = build_tree(full_path)
            # 即使子文件夹为空，也保留它，以便查看
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
    """处理根目录"""
    raw_tree = build_tree(local_path)
    
    # 预处理：为所有节点生成 ID 和 相对路径，方便前端索引
    def process_node(node, parent_path=""):
        node["id"] = str(hash(node["name"] + parent_path))[-8:] # 简单生成唯一ID
        
        if node["type"] == "file":
            rel_path = os.path.relpath(node["path"], local_path)
            # 这里只存相对路径，URL 编码在前端做，防止 Python 编码不兼容
            # 将 Windows 路径分隔符统一为 /
            safe_rel_path = "/".join(rel_path.split(os.sep))
            node["urlPath"] = f"{branch_name}/{safe_rel_path}"
            del node["path"]
        else:
            for child in node["children"]:
                process_node(child, parent_path + "/" + node["name"])

    process_node(raw_tree)
    return raw_tree["children"]

# 确保路径一致
data_cs = process_branch_root("docs/computer-science", "ComputerScience")
data_ls = process_branch_root("docs/life-science", "LifeScience")

# 构建完整数据结构
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
    <title>PKU Course Materials</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.10.1/jszip.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/FileSaver.js/2.0.5/FileSaver.min.js"></script>
    <style>
        :root { --primary: #0366d6; --bg: #f6f8fa; --border: #e1e4e8; --hover: #f1f8ff; --selected: #e1ecf4; }
        * { box-sizing: border-box; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; margin: 0; color: #24292e; display: flex; height: 100vh; overflow: hidden; }
        
        /* === 布局 === */
        .sidebar { width: 300px; border-right: 1px solid var(--border); background: #fff; display: flex; flex-direction: column; flex-shrink: 0; overflow: hidden; }
        .main-view { flex-grow: 1; display: flex; flex-direction: column; background: #fff; min-width: 0; }

        /* === 顶部栏 === */
        .header { height: 60px; border-bottom: 1px solid var(--border); display: flex; align-items: center; padding: 0 20px; justify-content: space-between; background: #fff; }
        .header h1 { font-size: 18px; margin: 0; }
        .actions { display: flex; gap: 10px; align-items: center; }

        .search-box { padding: 6px 12px; border: 1px solid var(--border); border-radius: 6px; font-size: 14px; width: 200px; }
        .btn { background-color: #2ea44f; color: white; border: 1px solid rgba(27,31,35,.15); padding: 6px 16px; font-size: 14px; font-weight: 600; border-radius: 6px; cursor: pointer; display: flex; align-items: center; gap: 5px; }
        .btn:disabled { background-color: #94d3a2; cursor: not-allowed; opacity: 0.7; }
        .btn:hover:not(:disabled) { background-color: #2c974b; }
        
        /* === 侧边栏树 === */
        .tree-container { flex-grow: 1; overflow-y: auto; padding: 10px 0; }
        .tree-item { padding: 4px 10px 4px 5px; cursor: pointer; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; display: flex; align-items: center; font-size: 14px; color: #444; }
        .tree-item:hover { background-color: var(--hover); }
        .tree-item.active { background-color: var(--selected); color: var(--primary); font-weight: 500; }
        .tree-indent { width: 16px; display: inline-block; flex-shrink: 0; }
        .tree-icon { margin-right: 6px; color: #888; }
        .tree-toggle { width: 20px; text-align: center; color: #999; font-size: 10px; transition: transform 0.15s; }
        .tree-toggle.open { transform: rotate(90deg); }
        .tree-toggle.invisible { visibility: hidden; }

        /* === 主视图表格 === */
        .file-list { flex-grow: 1; overflow-y: auto; padding: 0; }
        table { width: 100%; border-collapse: collapse; table-layout: fixed; }
        th { text-align: left; padding: 10px 15px; border-bottom: 1px solid var(--border); background: #fafbfc; color: #586069; font-size: 13px; font-weight: 600; position: sticky; top: 0; z-index: 1; }
        td { padding: 8px 15px; border-bottom: 1px solid #eaecef; font-size: 14px; vertical-align: middle; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
        tr:hover { background-color: var(--hover); }
        
        /* 列宽控制 */
        .col-check { width: 40px; text-align: center; }
        .col-icon { width: 30px; text-align: center; }
        .col-name { width: auto; }
        .col-size { width: 100px; text-align: right; color: #6a737d; font-family: monospace; }
        
        .icon-folder { color: #54aeff; }
        .icon-file { color: #6a737d; }
        
        /* 选中状态 */
        tr.selected { background-color: var(--selected); }
        
        /* 面包屑导航 */
        .breadcrumbs { padding: 10px 20px; font-size: 14px; color: #586069; border-bottom: 1px solid var(--border); display: flex; gap: 5px; overflow-x: auto; white-space: nowrap; }
        .crumb { cursor: pointer; color: var(--primary); }
        .crumb:hover { text-decoration: underline; }
        .crumb.current { color: #24292e; font-weight: 600; pointer-events: none; }
        .crumb-sep { color: #999; }

        /* 加载遮罩 */
        #loading-overlay { position: fixed; top: 0; left: 0; right: 0; bottom: 0; background: rgba(255,255,255,0.8); display: flex; flex-direction: column; align-items: center; justify-content: center; z-index: 100; display: none; }
        .spinner { width: 40px; height: 40px; border: 4px solid #f3f3f3; border-top: 4px solid var(--primary); border-radius: 50%; animation: spin 1s linear infinite; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        
        /* 移动端适配 */
        @media (max-width: 768px) {
            .sidebar { display: none; } /* 移动端暂藏侧边栏 */
            .col-size { display: none; }
        }
    </style>
</head>
<body>
    <div class="sidebar">
        <div style="padding: 15px; border-bottom: 1px solid var(--border); font-weight: bold;">📁 Course Explorer</div>
        <div class="tree-container" id="folder-tree"></div>
    </div>

    <div class="main-view">
        <div class="header">
            <h1 id="current-folder-name">PKU Course Hub</h1>
            <div class="actions">
                <input type="text" class="search-box" placeholder="Search (Coming soon)" disabled style="opacity: 0.5;">
                <button class="btn" id="download-btn" onclick="downloadSelected()" disabled>
                    <span>⬇</span> Download Selected
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
                <tbody id="file-table-body">
                    </tbody>
            </table>
        </div>
    </div>

    <div id="loading-overlay">
        <div class="spinner"></div>
        <div id="loading-text" style="margin-top: 15px; color: #555;">Processing...</div>
    </div>

    <script>
        const rawData = __DATA_JSON__;
        const BASE_URL = "__BASE_URL__";
        
        let currentFolder = null;
        let folderMap = new Map(); // 用于快速查找 ID -> Node
        let selectedFiles = new Set(); // 存储选中的文件对象

        // === 1. 初始化数据索引 ===
        function indexData(nodes, parentPath = []) {
            nodes.forEach(node => {
                node.parent = parentPath;
                if (node.id) folderMap.set(node.id, node);
                if (node.children) indexData(node.children, [...parentPath, node]);
            });
        }
        indexData(rawData);

        // === 2. 渲染左侧树 (只渲染文件夹) ===
        function renderTree(nodes, container, level = 0) {
            nodes.forEach(node => {
                // 只显示文件夹
                if (node.type !== 'folder') return;

                const div = document.createElement('div');
                div.className = 'tree-item';
                div.dataset.id = node.id;
                div.style.paddingLeft = (level * 15 + 5) + 'px';
                div.onclick = (e) => {
                    e.stopPropagation();
                    openFolder(node.id);
                };

                // 箭头
                const toggle = document.createElement('span');
                const hasSubFolders = node.children && node.children.some(c => c.type === 'folder');
                toggle.className = 'tree-toggle ' + (hasSubFolders ? '' : 'invisible');
                toggle.innerText = '▶';
                
                // 图标
                const icon = document.createElement('span');
                icon.className = 'tree-icon';
                icon.innerText = '📁';

                // 名称
                const name = document.createElement('span');
                name.innerText = node.name;

                div.append(toggle, icon, name);
                container.appendChild(div);

                // 子容器
                if (hasSubFolders) {
                    const subContainer = document.createElement('div');
                    subContainer.style.display = 'none'; // 默认折叠
                    renderTree(node.children, subContainer, level + 1);
                    container.appendChild(subContainer);
                    
                    // 折叠逻辑
                    toggle.onclick = (e) => {
                        e.stopPropagation();
                        const isOpen = subContainer.style.display === 'block';
                        subContainer.style.display = isOpen ? 'none' : 'block';
                        toggle.classList.toggle('open', !isOpen);
                    };
                }
            });
        }

        // === 3. 渲染右侧列表 ===
        function renderMain(folderNode) {
            const tbody = document.getElementById('file-table-body');
            tbody.innerHTML = '';
            
            // 更新面包屑
            updateBreadcrumbs(folderNode);
            document.getElementById('current-folder-name').innerText = folderNode.name || 'Root';
            
            // 如果是顶层列表，folderNode 是数组
            const items = Array.isArray(folderNode) ? folderNode : folderNode.children;

            if (!items || items.length === 0) {
                tbody.innerHTML = '<tr><td colspan="4" style="text-align:center; padding: 20px; color:#999;">Empty Folder</td></tr>';
                return;
            }

            // 排序：文件夹在前
            const sortedItems = [...items].sort((a, b) => {
                if (a.type === b.type) return a.name.localeCompare(b.name);
                return a.type === 'folder' ? -1 : 1;
            });

            sortedItems.forEach(item => {
                const tr = document.createElement('tr');
                
                // 复选框列
                const tdCheck = document.createElement('td');
                tdCheck.className = 'col-check';
                const checkbox = document.createElement('input');
                checkbox.type = 'checkbox';
                
                // 如果是文件，绑定选择逻辑；如果是文件夹，暂不支持选中(简化逻辑)
                if (item.type === 'file') {
                    checkbox.checked = selectedFiles.has(item);
                    checkbox.onchange = (e) => toggleSelection(item, e.target.checked);
                    tr.onclick = (e) => {
                        if (e.target !== checkbox) checkbox.click();
                    };
                } else {
                    checkbox.disabled = true; // 文件夹暂不批量选
                    checkbox.style.opacity = 0.3;
                    tr.onclick = () => openFolder(item.id); // 点击行进入文件夹
                }
                tdCheck.appendChild(checkbox);

                // 图标列
                const tdIcon = document.createElement('td');
                tdIcon.className = 'col-icon';
                tdIcon.innerHTML = item.type === 'folder' 
                    ? '<span class="icon-folder">📁</span>' 
                    : '<span class="icon-file">📄</span>';

                // 名称列
                const tdName = document.createElement('td');
                tdName.className = 'col-name';
                tdName.innerText = item.name;
                tdName.style.cursor = 'pointer';

                // 大小列
                const tdSize = document.createElement('td');
                tdSize.className = 'col-size';
                tdSize.innerText = item.type === 'folder' ? '-' : item.size + ' KB';

                tr.append(tdCheck, tdIcon, tdName, tdSize);
                tbody.appendChild(tr);
            });
            
            updateSelectAllState();
        }

        // === 4. 交互逻辑 ===
        
        function openFolder(id) {
            const node = folderMap.get(id);
            if (!node) return;
            
            currentFolder = node;
            
            // 高亮左侧
            document.querySelectorAll('.tree-item').forEach(el => el.classList.remove('active'));
            const activeTreeItem = document.querySelector(`.tree-item[data-id="${id}"]`);
            if (activeTreeItem) activeTreeItem.classList.add('active');

            renderMain(node);
        }

        function updateBreadcrumbs(node) {
            const container = document.getElementById('breadcrumbs');
            container.innerHTML = '';
            
            // 如果是数组(初始状态)，不显示面包屑
            if (Array.isArray(node)) return;

            const path = [...(node.parent || []), node];
            path.forEach((crumb, index) => {
                if (index > 0) {
                    const sep = document.createElement('span');
                    sep.className = 'crumb-sep';
                    sep.innerText = '/';
                    container.appendChild(sep);
                }
                
                const span = document.createElement('span');
                span.className = 'crumb';
                if (index === path.length - 1) span.classList.add('current');
                span.innerText = crumb.name;
                span.onclick = () => openFolder(crumb.id);
                container.appendChild(span);
            });
        }

        function toggleSelection(item, isChecked) {
            if (isChecked) selectedFiles.add(item);
            else selectedFiles.delete(item);
            updateBtnState();
            
            // 高亮选中行
            // 简单重新渲染可能太慢，这里直接操作DOM类名略复杂，暂略
        }
        
        function toggleSelectAll() {
            const selectAll = document.getElementById('select-all').checked;
            const items = currentFolder.children || [];
            
            items.forEach(item => {
                if (item.type === 'file') {
                    if (selectAll) selectedFiles.add(item);
                    else selectedFiles.delete(item);
                }
            });
            
            // 重新渲染列表复选框状态
            renderMain(currentFolder);
            updateBtnState();
        }
        
        function updateSelectAllState() {
            // 检查当前视图是否全选
            const items = currentFolder.children || [];
            const files = items.filter(i => i.type === 'file');
            if (files.length === 0) {
                document.getElementById('select-all').checked = false;
                document.getElementById('select-all').disabled = true;
                return;
            }
            document.getElementById('select-all').disabled = false;
            
            const allSelected = files.every(f => selectedFiles.has(f));
            document.getElementById('select-all').checked = allSelected;
        }

        function updateBtnState() {
            const btn = document.getElementById('download-btn');
            btn.disabled = selectedFiles.size === 0;
            btn.innerHTML = selectedFiles.size > 0 
                ? `<span>⬇</span> Download ${selectedFiles.size} Files`
                : `<span>⬇</span> Download Selected`;
        }

        // === 5. 下载逻辑 (修复 URL 编码) ===
        async function downloadSelected() {
            if (selectedFiles.size === 0) return;
            
            const overlay = document.getElementById('loading-overlay');
            const text = document.getElementById('loading-text');
            overlay.style.display = 'flex';
            
            const zip = new JSZip();
            let count = 0;
            const total = selectedFiles.size;
            
            try {
                for (const file of selectedFiles) {
                    text.innerText = `Fetching ${count + 1}/${total}: ${file.name}`;
                    
                    // 关键修复：URL 编码处理
                    // BASE_URL + 路径 (路径中每个部分都需要 encodeURIComponent)
                    const parts = file.urlPath.split('/');
                    const encodedPath = parts.map(p => encodeURIComponent(p)).join('/');
                    const url = `${BASE_URL}/${encodedPath}`;
                    
                    const response = await fetch(url);
                    if (!response.ok) throw new Error(`HTTP ${response.status}`);
                    const blob = await response.blob();
                    
                    zip.file(file.name, blob);
                    count++;
                }
                
                text.innerText = "Compressing...";
                const content = await zip.generateAsync({type: "blob"});
                saveAs(content, "PKU_Course_Materials.zip");
                
            } catch (err) {
                console.error(err);
                alert("Download failed: " + err.message + "\nPlease check console.");
            } finally {
                overlay.style.display = 'none';
                // 可选：下载后清空选择
                // selectedFiles.clear();
                // renderMain(currentFolder);
                // updateBtnState();
            }
        }

        // === 启动 ===
        const rootContainer = document.getElementById('folder-tree');
        renderTree(rawData, rootContainer);
        
        // 默认打开第一个根目录
        if (rawData.length > 0) {
             openFolder(rawData[0].id);
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

print("Explorer-style Index generated!")