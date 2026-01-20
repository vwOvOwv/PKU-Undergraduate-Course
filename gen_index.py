import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
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
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME", "README.md"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            child_tree = build_tree(full_path)
            # 只有当子文件夹有内容时才展示
            if child_tree["children"]: 
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
    
    def fix_urls(node):
        if node["type"] == "folder":
            for child in node["children"]:
                fix_urls(child)
        else:
            rel_path = os.path.relpath(node["path"], local_path)
            # 路径转义
            safe_path = "/".join([p for p in rel_path.split(os.sep)])
            node["url"] = f"{BASE_URL}@{branch_name}/{safe_path}"
            del node["path"]

    fix_urls(raw_tree)
    return raw_tree["children"]

# 确保这里的路径和你 deploy.yml 里 checkout 的路径一致
data_cs = process_branch_root("docs/computer-science", "ComputerScience")
data_ls = process_branch_root("docs/life-science", "LifeScience")

full_data = [
    {"name": "Computer Science", "type": "folder", "children": data_cs},
    {"name": "Life Science", "type": "folder", "children": data_ls}
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
        :root { --primary: #0366d6; --bg: #f6f8fa; --border: #e1e4e8; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; margin: 0; color: #24292e; display: flex; height: 100vh; overflow: hidden; }
        
        .sidebar { width: 350px; border-right: 1px solid var(--border); display: flex; flex-direction: column; background: #fff; flex-shrink: 0; }
        .main-content { flex-grow: 1; padding: 40px; overflow-y: auto; background: var(--bg); display: flex; align-items: center; justify-content: center; color: #586069; }

        .controls { padding: 15px; border-bottom: 1px solid var(--border); background: #fff; z-index: 10; }
        h1 { margin: 0 0 10px 0; font-size: 18px; display: flex; align-items: center; }
        h1 span { font-size: 12px; background: #eee; padding: 2px 6px; border-radius: 4px; margin-left: 10px; color: #666; font-weight: normal; }

        .search-box { width: 100%; padding: 8px; border: 1px solid var(--border); border-radius: 6px; box-sizing: border-box; margin-bottom: 10px; }
        .btn { width: 100%; background-color: #2ea44f; color: white; border: 1px solid rgba(27,31,35,.15); padding: 8px 16px; font-size: 14px; font-weight: 600; border-radius: 6px; cursor: pointer; text-align: center; }
        .btn:disabled { background-color: #94d3a2; cursor: not-allowed; }
        .progress { font-size: 12px; color: #666; margin-top: 5px; text-align: center; height: 16px; }

        #file-tree { flex-grow: 1; overflow-y: auto; padding: 10px; }
        ul { list-style: none; padding-left: 18px; margin: 0; }
        li { margin: 2px 0; }
        
        .row { display: flex; align-items: center; padding: 4px; border-radius: 4px; white-space: nowrap; cursor: pointer; }
        .row:hover { background-color: #f1f8ff; }

        .toggle { width: 20px; height: 20px; display: inline-flex; align-items: center; justify-content: center; margin-right: 2px; transition: transform 0.2s ease; color: #6a737d; font-size: 12px; }
        .toggle::before { content: "▶"; }
        li.expanded > .row .toggle { transform: rotate(90deg); }
        li.empty > .row .toggle { visibility: hidden; }

        input[type="checkbox"] { margin-right: 8px; }
        .icon { margin-right: 6px; }
        .name { flex-grow: 1; font-size: 14px; overflow: hidden; text-overflow: ellipsis; }
        .meta { font-size: 12px; color: #999; margin-left: 8px; }

        /* === 关键修复：只隐藏嵌套的子列表，不隐藏根列表 === */
        ul.nested { display: none; }
        li.expanded > ul.nested { display: block; }
        .hidden { display: none !important; }
        
        /* 移动端适配 */
        @media (max-width: 768px) {
            body { flex-direction: column; overflow: auto; }
            .sidebar { width: 100%; border-right: none; }
            .main-content { display: none; }
        }
    </style>
</head>
<body>
    <div class="sidebar">
        <div class="controls">
            <h1>PKU Course Hub <span id="count-badge">0 files</span></h1>
            <input type="text" id="search" class="search-box" placeholder="Search..." oninput="filterTree(this.value)">
            <button id="downloadBtn" class="btn" onclick="downloadZip()" disabled>Download ZIP</button>
            <div id="progress" class="progress"></div>
        </div>
        <div id="file-tree"></div>
    </div>

    <div class="main-content">
        <div style="text-align: center;">
            <h2>Welcome to Course Material Hub</h2>
            <p>Select files from the left sidebar to download.</p>
        </div>
    </div>

    <script>
        const treeData = __DATA_JSON__;
        
        function createTree(nodes, parentElement) {
            const ul = document.createElement('ul');
            
            // === 修复点：只有当父级不是根容器时，才添加 'nested' 类 ===
            if (parentElement.id !== 'file-tree') {
                ul.className = 'nested';
            } else {
                ul.className = 'root-list';
            }
            
            nodes.forEach(node => {
                const li = document.createElement('li');
                const row = document.createElement('div');
                row.className = 'row';
                
                const toggle = document.createElement('span');
                toggle.className = 'toggle';
                if (node.type !== 'folder') toggle.style.visibility = 'hidden';
                
                const checkbox = document.createElement('input');
                checkbox.type = 'checkbox';
                checkbox.className = node.type === 'folder' ? 'folder-check' : 'file-check';
                checkbox.dataset.url = node.url || '';
                checkbox.dataset.name = node.name;
                checkbox.onclick = (e) => {
                    e.stopPropagation();
                    if (node.type === 'folder') toggleChildren(li, checkbox.checked);
                    updateBtnState();
                };

                const icon = document.createElement('span');
                icon.className = 'icon';
                icon.innerText = node.type === 'folder' ? '📁' : '📄';
                
                const name = document.createElement('span');
                name.className = 'name';
                name.innerText = node.name;
                
                row.append(toggle, checkbox, icon, name);
                
                if (node.type === 'folder') {
                    row.onclick = () => li.classList.toggle('expanded');
                } else {
                    const size = document.createElement('span');
                    size.className = 'meta';
                    size.innerText = node.size + 'KB';
                    row.appendChild(size);
                    row.onclick = (e) => {
                        if(e.target !== checkbox) window.open(node.url, '_blank');
                    }
                }

                li.appendChild(row);

                if (node.children && node.children.length > 0) {
                    createTree(node.children, li);
                } else if (node.type === 'folder') {
                    li.classList.add('empty');
                }
                
                ul.appendChild(li);
            });
            parentElement.appendChild(ul);
        }

        function toggleChildren(li, isChecked) {
            li.querySelectorAll('input[type="checkbox"]').forEach(cb => cb.checked = isChecked);
        }

        function updateBtnState() {
            const checked = document.querySelectorAll('.file-check:checked');
            document.getElementById('downloadBtn').disabled = checked.length === 0;
            document.getElementById('count-badge').innerText = `${checked.length} selected`;
        }

        function filterTree(text) {
            const term = text.toLowerCase();
            document.querySelectorAll('li').forEach(li => {
                const name = li.querySelector('.name').innerText.toLowerCase();
                if (!term) {
                    li.classList.remove('hidden');
                    return;
                }
                if (name.includes(term)) {
                    li.classList.remove('hidden');
                    // 展开父级
                    let parent = li.parentElement;
                    while(parent && parent.id !== 'file-tree') {
                        if(parent.tagName === 'UL') parent.parentElement.classList.add('expanded');
                        parent.parentElement.classList.remove('hidden');
                        parent = parent.parentElement;
                    }
                } else {
                    li.classList.add('hidden');
                }
            });
        }

        async function downloadZip() {
            const checked = document.querySelectorAll('.file-check:checked');
            if (checked.length === 0) return;
            const zip = new JSZip();
            const btn = document.getElementById('downloadBtn');
            const progress = document.getElementById('progress');
            btn.disabled = true;
            
            try {
                let count = 0;
                for (const cb of checked) {
                    progress.innerText = `Fetching ${++count}/${checked.length}`;
                    const response = await fetch(cb.dataset.url);
                    if (!response.ok) throw new Error('Network error');
                    zip.file(cb.dataset.name, await response.blob());
                }
                progress.innerText = "Compressing...";
                saveAs(await zip.generateAsync({type:"blob"}), "PKU_Course_Materials.zip");
                progress.innerText = "Done!";
            } catch (err) {
                alert("Download failed. Check console for details.");
            } finally {
                btn.disabled = false;
                updateBtnState();
            }
        }

        const rootUl = document.getElementById('file-tree');
        createTree(treeData, rootUl);
        // 默认展开第一层级
        rootUl.querySelectorAll('#file-tree > ul > li').forEach(li => li.classList.add('expanded'));
    </script>
</body>
</html>
"""

# 写入文件
with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(html_template.replace("__DATA_JSON__", json.dumps(full_data)))

print("Fixed Index generated!")