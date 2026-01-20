import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
BASE_URL = f"https://github.com/{USER}/{REPO}/raw"

def build_tree(path):
    """递归构建文件树"""
    tree = {"name": os.path.basename(path), "type": "folder", "children": []}
    
    try:
        # 获取该目录下所有内容
        items = os.listdir(path)
    except FileNotFoundError:
        return tree

    # 排序：文件夹在前，文件在后
    items.sort(key=lambda x: (not os.path.isdir(os.path.join(path, x)), x))

    for item in items:
        # ❌ 过滤规则：忽略 .git, .github, 以及脚本自身
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            # 递归处理子文件夹
            child_tree = build_tree(full_path)
            # 只有当子文件夹里有东西时才添加（避免空文件夹占位）
            if child_tree["children"]: 
                tree["children"].append(child_tree)
        else:
            # 处理文件
            # 计算用于下载的相对路径
            # 这里的逻辑是：去掉 docs/computer-science/ 前缀，还要补上分支名
            # 为了简化，我们在生成 HTML 时再拼接最终 URL，这里只存相对路径
            tree["children"].append({
                "name": item,
                "type": "file",
                "path": full_path, # 暂存完整路径，稍后处理
                "size": round(os.path.getsize(full_path) / 1024, 2)
            })
            
    return tree

def process_branch_root(local_path, branch_name):
    """处理根目录，修正下载链接"""
    raw_tree = build_tree(local_path)
    
    # 修正树中所有文件的下载链接
    def fix_urls(node):
        if node["type"] == "folder":
            for child in node["children"]:
                fix_urls(child)
        else:
            # 计算相对于 local_path 的路径
            # 例如: docs/computer-science/Course/1.pdf -> Course/1.pdf
            rel_path = os.path.relpath(node["path"], local_path)
            # 生成最终下载链接
            node["url"] = f"{BASE_URL}/{branch_name}/{rel_path}"
            # 清理掉 path 字段，减小 JSON 体积
            del node["path"]

    fix_urls(raw_tree)
    return raw_tree["children"] # 返回 children 列表，去掉顶层的 root 包裹

# 生成数据
data_cs = process_branch_root("docs/computer-science", "ComputerScience")
data_ls = process_branch_root("docs/life-science", "LifeScience")

full_data = [
    {"name": "Computer Science", "type": "folder", "children": data_cs},
    {"name": "Life Science", "type": "folder", "children": data_ls}
]

# HTML 模板 (包含 CSS 和 JS)
html_template = """
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PKU Course Materials Downloader</title>
    <style>
        :root { --primary: #0366d6; --bg: #f6f8fa; --border: #e1e4e8; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; max-width: 900px; margin: 0 auto; padding: 20px; color: #24292e; }
        
        .header { display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid var(--border); padding-bottom: 20px; margin-bottom: 20px; }
        h1 { margin: 0; font-size: 24px; }
        
        /* 按钮样式 */
        .btn { background-color: #2ea44f; color: white; border: 1px solid rgba(27,31,35,.15); padding: 6px 16px; font-size: 14px; font-weight: 600; border-radius: 6px; cursor: pointer; transition: .2s; }
        .btn:hover { background-color: #2c974b; }
        .btn:disabled { background-color: #94d3a2; cursor: not-allowed; }
        
        /* 树状结构样式 */
        ul { list-style: none; padding-left: 20px; margin: 0; }
        li { margin: 4px 0; }
        
        .folder-row, .file-row { display: flex; align-items: center; padding: 4px 8px; border-radius: 4px; }
        .folder-row:hover, .file-row:hover { background-color: var(--bg); }
        
        /* 图标与交互 */
        .toggle { cursor: pointer; user-select: none; margin-right: 5px; width: 16px; text-align: center; color: #6a737d; }
        .toggle:hover { color: var(--primary); }
        .icon { margin-right: 8px; }
        
        .folder-name { font-weight: 600; cursor: pointer; flex-grow: 1; }
        .file-link { text-decoration: none; color: var(--primary); flex-grow: 1; }
        .file-link:hover { text-decoration: underline; }
        
        .meta { font-size: 12px; color: #6a737d; margin-left: 10px; min-width: 60px; text-align: right; }
        
        /* 复选框 */
        input[type="checkbox"] { margin-right: 10px; cursor: pointer; }
        
        .hidden { display: none; }
        .collapsed .children { display: none; }
        .collapsed .toggle::before { content: "▶"; font-size: 10px; }
        .expanded .toggle::before { content: "▼"; font-size: 10px; }
    </style>
</head>
<body>
    <div class="header">
        <div>
            <h1>PKU Course Hub</h1>
            <p style="color: #586069; margin: 5px 0 0 0;">Select files to download in bulk.</p>
        </div>
        <button id="downloadBtn" class="btn" onclick="downloadSelected()" disabled>Download Selected (0)</button>
    </div>
    
    <div id="file-tree"></div>

    <script>
        const treeData = __DATA_JSON__;
        
        function createTree(nodes, parentElement) {
            const ul = document.createElement('ul');
            ul.className = 'children';
            
            nodes.forEach(node => {
                const li = document.createElement('li');
                
                if (node.type === 'folder') {
                    li.className = 'expanded'; // 默认展开
                    
                    const row = document.createElement('div');
                    row.className = 'folder-row';
                    
                    // 1. 折叠箭头
                    const toggle = document.createElement('span');
                    toggle.className = 'toggle';
                    toggle.onclick = (e) => {
                        li.classList.toggle('expanded');
                        li.classList.toggle('collapsed');
                        e.stopPropagation();
                    };
                    
                    // 2. 文件夹复选框 (全选子文件)
                    const checkbox = document.createElement('input');
                    checkbox.type = 'checkbox';
                    checkbox.onclick = (e) => toggleFolder(e.target, li);
                    
                    // 3. 文件夹图标与名称
                    const icon = document.createElement('span');
                    icon.className = 'icon';
                    icon.innerText = '📁';
                    
                    const name = document.createElement('span');
                    name.className = 'folder-name';
                    name.innerText = node.name;
                    name.onclick = () => toggle.click(); // 点击名字也能折叠
                    
                    row.append(toggle, checkbox, icon, name);
                    li.appendChild(row);
                    
                    // 递归创建子节点
                    if (node.children) {
                        createTree(node.children, li);
                    }
                    
                } else {
                    const row = document.createElement('div');
                    row.className = 'file-row';
                    
                    // 占位符(为了对齐)
                    const spacer = document.createElement('span');
                    spacer.className = 'toggle';
                    
                    // 1. 文件复选框
                    const checkbox = document.createElement('input');
                    checkbox.type = 'checkbox';
                    checkbox.className = 'file-check';
                    checkbox.value = node.url;
                    checkbox.onchange = updateBtnState;
                    
                    // 2. 文件链接
                    const icon = document.createElement('span');
                    icon.className = 'icon';
                    icon.innerText = '📄';
                    
                    const link = document.createElement('a');
                    link.className = 'file-link';
                    link.href = node.url;
                    link.innerText = node.name;
                    link.target = "_blank";
                    
                    const size = document.createElement('span');
                    size.className = 'meta';
                    size.innerText = node.size + ' KB';
                    
                    row.append(spacer, checkbox, icon, link, size);
                    li.appendChild(row);
                }
                ul.appendChild(li);
            });
            parentElement.appendChild(ul);
        }

        // 文件夹全选/反选逻辑
        function toggleFolder(folderCheckbox, liItem) {
            const allChecks = liItem.querySelectorAll('input[type="checkbox"]');
            allChecks.forEach(cb => cb.checked = folderCheckbox.checked);
            updateBtnState();
        }

        // 更新按钮状态
        function updateBtnState() {
            const checked = document.querySelectorAll('.file-check:checked');
            const btn = document.getElementById('downloadBtn');
            btn.innerText = `Download Selected (${checked.length})`;
            btn.disabled = checked.length === 0;
        }

        // 批量下载逻辑
        function downloadSelected() {
            const checked = document.querySelectorAll('.file-check:checked');
            if (checked.length === 0) return;
            
            if (!confirm(`Ready to download ${checked.length} files?\\n\\n⚠️ IMPORTANT:\\nYour browser may block multiple popups. Please check the address bar to allow popups for this site.`)) {
                return;
            }

            // 间隔下载，防止浏览器崩溃
            checked.forEach((cb, index) => {
                setTimeout(() => {
                    const link = document.createElement('a');
                    link.href = cb.value;
                    link.download = ''; // 尝试触发下载
                    document.body.appendChild(link);
                    link.click();
                    document.body.removeChild(link);
                }, index * 800); // 800ms 间隔
            });
        }

        // 初始化
        createTree(treeData, document.getElementById('file-tree'));
    </script>
</body>
</html>
"""

# 写入文件
with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(html_template.replace("__DATA_JSON__", json.dumps(full_data)))

print("Tree index generated successfully!")