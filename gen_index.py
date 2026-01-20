import os
import json

# 配置你的仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
BASE_URL = f"https://github.com/{USER}/{REPO}/raw"

def scan_dir(path, branch_name, display_root):
    tree = {"name": display_root, "type": "folder", "children": []}
    
    # 遍历目录
    for root, dirs, files in os.walk(path):
        # 排序，让文件夹在文件前面
        dirs.sort()
        files.sort()
        
        # 获取当前文件夹在树中的位置
        rel_path = os.path.relpath(root, path)
        if rel_path == ".":
            current_node = tree
        else:
            # 简单的路径查找（假设是按顺序walk的，其实可以用字典优化，这里简化处理）
            parts = rel_path.split(os.sep)
            current_node = tree
            for part in parts:
                found = False
                for child in current_node["children"]:
                    if child["name"] == part and child["type"] == "folder":
                        current_node = child
                        found = True
                        break
                if not found:
                    new_folder = {"name": part, "type": "folder", "children": []}
                    current_node["children"].append(new_folder)
                    current_node = new_folder

        # 添加文件
        for f in files:
            # 忽略隐藏文件和脚本自身
            if f.startswith(".") or f == "index.html" or f.endswith(".py"):
                continue
                
            full_path = os.path.join(root, f)
            # 构建下载链接: https://github.com/User/Repo/raw/Branch/Path
            # 注意：这里要把 docs/cs/ 前缀去掉，变成仓库根目录的相对路径
            clean_path = os.path.relpath(full_path, path) 
            download_url = f"{BASE_URL}/{branch_name}/{clean_path}"
            
            current_node["children"].append({
                "name": f,
                "type": "file",
                "url": download_url,
                "size": round(os.path.getsize(full_path) / 1024, 2) # KB
            })
            
    return tree

# 扫描两个目录
data_cs = scan_dir("docs/cs", "ComputerScience", "Computer Science")
data_ls = scan_dir("docs/ls", "LifeScience", "Life Science")

# 合并数据
full_data = [data_cs, data_ls]

# 生成简单的 HTML
html_template = """
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>PKU Course Material Downloader</title>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; max-width: 1000px; margin: 0 auto; padding: 20px; }
        .header { display: flex; justify-content: space-between; align-items: center; border-bottom: 2px solid #eee; padding-bottom: 20px; margin-bottom: 20px; }
        .btn { background: #007bff; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; font-size: 16px; }
        .btn:hover { background: #0056b3; }
        .btn:disabled { background: #ccc; cursor: not-allowed; }
        ul { list-style: none; padding-left: 20px; }
        li { margin: 5px 0; }
        .folder { cursor: pointer; font-weight: bold; color: #333; }
        .folder::before { content: "📁 "; }
        .file { color: #666; }
        .file a { text-decoration: none; color: #007bff; }
        .file a:hover { text-decoration: underline; }
        .hidden { display: none; }
        .check-box { margin-right: 10px; }
    </style>
</head>
<body>
    <div class="header">
        <h1>PKU Undergraduate Course Materials</h1>
        <button id="downloadBtn" class="btn" onclick="downloadSelected()" disabled>Download Selected (0)</button>
    </div>
    
    <div id="file-tree"></div>

    <script>
        const data = __DATA_JSON__;
        
        function createTree(nodes, parentElement) {
            const ul = document.createElement('ul');
            if (parentElement.id !== 'file-tree') ul.classList.add('hidden'); // 默认折叠子文件夹
            
            nodes.forEach(node => {
                const li = document.createElement('li');
                
                if (node.type === 'folder') {
                    const span = document.createElement('span');
                    span.className = 'folder';
                    span.innerText = node.name;
                    span.onclick = function(e) {
                        // 展开/折叠
                        const childUl = this.nextElementSibling;
                        if (childUl) childUl.classList.toggle('hidden');
                        e.stopPropagation();
                    };
                    li.appendChild(span);
                    if (node.children.length > 0) {
                        createTree(node.children, li);
                    }
                } else {
                    // 文件复选框
                    const checkbox = document.createElement('input');
                    checkbox.type = 'checkbox';
                    checkbox.className = 'check-box';
                    checkbox.value = node.url;
                    checkbox.onchange = updateCount;
                    
                    const link = document.createElement('a');
                    link.href = node.url;
                    link.innerText = `${node.name} (${node.size} KB)`;
                    link.target = "_blank";
                    
                    const div = document.createElement('div');
                    div.className = 'file';
                    div.appendChild(checkbox);
                    div.appendChild(link);
                    li.appendChild(div);
                }
                ul.appendChild(li);
            });
            parentElement.appendChild(ul);
        }

        function updateCount() {
            const checked = document.querySelectorAll('.check-box:checked');
            const btn = document.getElementById('downloadBtn');
            btn.innerText = `Download Selected (${checked.length})`;
            btn.disabled = checked.length === 0;
        }

        function downloadSelected() {
            const checked = document.querySelectorAll('.check-box:checked');
            if (confirm(`Ready to download ${checked.length} files? Note: Browsers may block multiple downloads. Please allow popups.`)) {
                checked.forEach((box, index) => {
                    setTimeout(() => {
                        const a = document.createElement('a');
                        a.href = box.value;
                        a.download = '';
                        document.body.appendChild(a);
                        a.click();
                        document.body.removeChild(a);
                    }, index * 500); // 间隔500毫秒防止卡死
                });
            }
        }

        // 初始化
        createTree(data, document.getElementById('file-tree'));
    </script>
</body>
</html>
"""

# 写入文件
with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(html_template.replace("__DATA_JSON__", json.dumps(full_data)))

print("Index generated successfully!")