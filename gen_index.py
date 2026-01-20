import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
BASE_URL = f"https://github.com/{USER}/{REPO}/raw"

def scan_dir(path, branch_name):
    tree = []
    # 确保路径存在，防止报错
    if not os.path.exists(path):
        print(f"Warning: Path not found {path}")
        return []

    for root, dirs, files in os.walk(path):
        dirs.sort()
        files.sort()
        
        # 忽略隐藏文件
        files = [f for f in files if not f.startswith(".") and f != "index.html"]
        
        for f in files:
            full_path = os.path.join(root, f)
            # 关键：计算相对路径，用于生成链接
            # 例如: docs/computer-science/Course/1.pdf -> Course/1.pdf
            rel_path = os.path.relpath(full_path, path)
            
            # 生成 GitHub Raw 链接
            # 链接结构: BASE_URL / 分支名 / 文件相对路径
            download_url = f"{BASE_URL}/{branch_name}/{rel_path}"
            
            # 简单把所有文件扁平化展示，或者保留层级（这里为了简便，生成简单的列表项）
            tree.append({
                "name": f"{rel_path}", # 显示带文件夹的路径
                "url": download_url,
                "size": round(os.path.getsize(full_path) / 1024, 2)
            })
    return tree

# === 关键修改：这里要匹配 YAML 里的 path ===
data_cs = scan_dir("docs/computer-science", "ComputerScience")
data_ls = scan_dir("docs/life-science", "LifeScience")

full_data = {"CS": data_cs, "LS": data_ls}

# 生成 HTML
html = """
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>PKU Course Materials</title>
    <style>
        body { font-family: sans-serif; max-width: 800px; margin: 20px auto; line-height: 1.6; }
        h1, h2 { border-bottom: 1px solid #eee; padding-bottom: 10px; }
        .file-item { margin: 5px 0; }
        a { text-decoration: none; color: #0366d6; }
        a:hover { text-decoration: underline; }
        .size { color: #666; font-size: 0.85em; margin-left: 10px; }
    </style>
</head>
<body>
    <h1>PKU Course Materials Index</h1>
    <p>Click files to download directly from GitHub.</p>
    
    <h2>Computer Science</h2>
    <div id="cs-list"></div>
    
    <h2>Life Science</h2>
    <div id="ls-list"></div>

    <script>
        const data = __DATA__;
        
        function render(list, containerId) {
            const container = document.getElementById(containerId);
            if (list.length === 0) {
                container.innerHTML = "<p>No files found.</p>";
                return;
            }
            let html = "";
            list.forEach(item => {
                html += `<div class="file-item">
                    <a href="${item.url}" target="_blank">📄 ${item.name}</a>
                    <span class="size">(${item.size} KB)</span>
                </div>`;
            });
            container.innerHTML = html;
        }
        
        render(data.CS, "cs-list");
        render(data.LS, "ls-list");
    </script>
</body>
</html>
"""

# 写入文件
with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(html.replace("__DATA__", json.dumps(full_data)))

print("Index generated successfully!")