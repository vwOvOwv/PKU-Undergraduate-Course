import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
# 使用 jsDelivr CDN 加速 raw 文件下载，且通常有更好的 CORS 支持
BASE_URL = f"https://cdn.jsdelivr.net/gh/{USER}/{REPO}"

def build_tree(path):
    """递归构建文件树"""
    tree = {"name": os.path.basename(path), "type": "folder", "children": []}
    try:
        items = os.listdir(path)
    except FileNotFoundError:
        return tree

    # 排序：文件夹在前
    items.sort(key=lambda x: (not os.path.isdir(os.path.join(path, x)), x))

    for item in items:
        # 过滤规则
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            child_tree = build_tree(full_path)
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
    """处理根目录，生成 CDN 链接"""
    raw_tree = build_tree(local_path)
    
    def fix_urls(node):
        if node["type"] == "folder":
            for child in node["children"]:
                fix_urls(child)
        else:
            rel_path = os.path.relpath(node["path"], local_path)
            # 路径转义，处理空格和特殊字符
            safe_path = "/".join([p for p in rel_path.split(os.sep)])
            node["url"] = f"{BASE_URL}@{branch_name}/{safe_path}"
            del node["path"]

    fix_urls(raw_tree)
    return raw_tree["children"]

# 生成数据
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
        
        /* 侧边栏布局 */
        .sidebar { width: 350px; border-right: 1px solid var(--border); display: flex; flex-direction: column; background: #fff; flex-shrink: 0; }
        .main-content { flex-grow: 1; padding: 40px; overflow-y: auto; background: var(--bg); display: flex; align-items: center; justify-content: center; color: #586069; }

        /* 顶部控制区 */
        .controls { padding: 15px; border-bottom: 1px solid var(--border); background: #fff; z-index: 10; box-shadow: 0 2px 5px rgba(0,0,0,0.05); }
        
        h1 { margin: 0 0 10px 0; font-size: 18px; display: flex; align-items: center; }
        h1 span { font-size: 12px; background: #eee; padding: 2px 6px; border-radius: 4px; margin-left: 10px; color: #666; font-weight: normal; }

        /* 搜索框 */
        .search-box { width: 100%; padding: 8px; border: 1px solid var(--border); border-radius: 6px; box-sizing: border-box; margin-bottom: 10px; font-size: 14px; }
        .search-box:focus { border-color: var(--primary); outline: none; box-shadow: 0 0 0 3px rgba(3,102,214,0.3); }

        /* 按钮 */
        .btn { width: 100%; background-color: #2ea44f; color: white; border: 1px solid rgba(27,31,35,.15); padding: 8px 16px; font-size: 14px; font-weight: 600; border-radius: 6px; cursor: pointer; transition: .2s; text-align: center; }
        .btn:hover { background-color: #2c974b; }
        .btn:disabled { background-color: #94d3a2; cursor: not-allowed; opacity: 0.7; }
        .progress { font-size: 12px; color: #666; margin-top: 5px; text-align: center; height: 16px; }

        /* 文件树区域 */
        #file-tree { flex-grow: 1; overflow-y: auto; padding: 10px; }
        ul { list-style: none; padding-left: 18px; margin: 0; }
        li { margin: 2px 0; }
        
        .row { display: flex; align-items: center; padding: 4px; border-radius: 4px; white-space: nowrap; cursor: pointer; }
        .row:hover { background-color: #f1f8ff; }
        .row.selected { background-color: #e1ecf4; }

        /* 箭头逻辑 - 关键修复 */
        .toggle { width: 20px; height: 20px; display: inline-flex; align-items: center; justify-content: center; margin-right: 2px; transition: transform 0.2s ease; color: #6a737d; font-size: 12px; }
        .toggle::before { content: "▶"; }
        /* 展开状态：旋转90度 */
        li.expanded > .row .toggle { transform: rotate(90deg); }
        /* 隐藏空文件夹箭头 */
        li.empty > .row .toggle { visibility: hidden; }

        input[type="checkbox"] { margin-right: 8px; cursor: pointer; }
        .icon { margin-right: 6px; }
        .name { flex-grow: 1; font-size: 14px; overflow: hidden; text-overflow: ellipsis; }
        .meta { font-size: 12px; color: #999; margin-left: 8px; min-width: 50px; text-align: right; }

        /* 隐藏逻辑 */
        ul.children { display: none; }
        li.expanded > ul.children { display: block; }
        .hidden { display: none !important; }

        /* 响应式 */
        @media (max-width: 768px) {
            body { flex-direction: column; overflow: auto; }
            .sidebar { width: 100%; height: auto; border-right: none; }
            .main-content { display: none; }
        }
    </style>
</head>
<body>
    <div class="sidebar">
        <div class="controls">
            <h1>PKU Course Hub <span id="count-badge">0 files</span></h1>
            <input type="text" id="search" class="search-box" placeholder="Search files or folders..." oninput="filterTree(this.value)">
            <button id="downloadBtn" class="btn" onclick="downloadZip()" disabled>Download Selected as ZIP</button>
            <div id="progress" class="progress"></div>
        </div>
        <div id="file-tree"></div>
    </div>

    <div class="main-content">
        <div style="text-align: center;">
            <h2 style="color: #333;">Welcome to Course Material Hub</h2>
            <p>Select files from the left sidebar to download.</p>
            <p style="font-size: 0.9em; color: #666;">Supports bulk downloading via client-side compression.</p>
        </div>
    </div>

    <script>
        const treeData = __DATA_JSON__;
        
        function createTree(nodes, parentElement) {
            const ul = document.createElement('ul');
            ul.className = 'children';
            
            nodes.forEach(node => {
                const li = document.createElement('li');
                // 默认全部为收起状态 (没有 expanded class)
                
                const row = document.createElement('div');
                row.className = 'row';
                
                // 1. 箭头 (仅文件夹)
                const toggle = document.createElement('span');
                toggle.className = 'toggle';
                if (node.type !== 'folder') toggle.style.visibility = 'hidden';
                
                // 2. 复选框
                const checkbox = document.createElement('input');
                checkbox.type = 'checkbox';
                checkbox.className = node.type === 'folder' ? 'folder-check' : 'file-check';
                checkbox.dataset.url = node.url || '';
                checkbox.dataset.name = node.name;
                checkbox.onclick = (e) => {
                    e.stopPropagation(); // 防止触发折叠
                    if (node.type === 'folder') toggleChildren(li, checkbox.checked);
                    updateBtnState();
                };

                // 3. 图标与名称
                const icon = document.createElement('span');
                icon.className = 'icon';
                icon.innerText = node.type === 'folder' ? '📁' : '📄';
                
                const name = document.createElement('span');
                name.className = 'name';
                name.innerText = node.name;
                
                row.append(toggle, checkbox, icon, name);
                
                // 文件夹点击事件：展开/收起
                if (node.type === 'folder') {
                    row.onclick = () => {
                        li.classList.toggle('expanded');
                    };
                } else {
                    // 文件显示大小
                    const size = document.createElement('span');
                    size.className = 'meta';
                    size.innerText = node.size + 'KB';
                    row.appendChild(size);
                    // 文件点击：在新标签预览/下载
                    row.onclick = (e) => {
                        // 阻止行点击触发checkbox
                        if(e.target !== checkbox) {
                             window.open(node.url, '_blank');
                        }
                    }
                }

                li.appendChild(row);

                // 递归处理子节点
                if (node.children && node.children.length > 0) {
                    createTree(node.children, li);
                } else if (node.type === 'folder') {
                    li.classList.add('empty'); // 空文件夹标记
                }
                
                ul.appendChild(li);
            });
            parentElement.appendChild(ul);
        }

        // 文件夹全选/反选逻辑 (递归)
        function toggleChildren(li, isChecked) {
            const children = li.querySelectorAll('input[type="checkbox"]');
            children.forEach(cb => cb.checked = isChecked);
        }

        // 更新按钮状态
        function updateBtnState() {
            const checked = document.querySelectorAll('.file-check:checked');
            const btn = document.getElementById('downloadBtn');
            const badge = document.getElementById('count-badge');
            
            btn.innerText = checked.length > 0 ? `Download ZIP (${checked.length})` : 'Download Selected as ZIP';
            btn.disabled = checked.length === 0;
            badge.innerText = `${checked.length} selected`;
        }

        // 搜索功能
        function filterTree(text) {
            const term = text.toLowerCase();
            const allLi = document.querySelectorAll('li');
            
            if (!term) {
                // 清除搜索：恢复默认收起状态，或全部展开，这里选择全部移除 hidden
                allLi.forEach(li => {
                    li.classList.remove('hidden');
                    // 可选：搜索清空时收起所有
                    if (li.querySelector('ul')) li.classList.remove('expanded');
                });
                return;
            }

            allLi.forEach(li => {
                const nameSpan = li.querySelector('.name');
                const name = nameSpan ? nameSpan.innerText.toLowerCase() : '';
                // 如果匹配，显示该节点
                if (name.includes(term)) {
                    li.classList.remove('hidden');
                    // 关键：展开所有父级
                    let parent = li.parentElement; // ul
                    while (parent && parent.id !== 'file-tree') {
                        if (parent.tagName === 'UL') {
                            const parentLi = parent.parentElement;
                            if (parentLi) {
                                parentLi.classList.remove('hidden');
                                parentLi.classList.add('expanded');
                            }
                        }
                        parent = parent.parentElement;
                    }
                } else {
                    // 如果不匹配，先隐藏。
                    // 注意：稍后如果发现子节点有匹配的，会重新显示父节点(上面的 while 逻辑)
                    li.classList.add('hidden');
                }
            });
        }

        // === 核心：ZIP 打包下载逻辑 ===
        async function downloadZip() {
            const checked = document.querySelectorAll('.file-check:checked');
            if (checked.length === 0) return;

            const zip = new JSZip();
            const btn = document.getElementById('downloadBtn');
            const progress = document.getElementById('progress');
            
            btn.disabled = true;
            btn.innerText = "Packing...";
            
            let count = 0;
            const total = checked.length;

            try {
                for (const cb of checked) {
                    const url = cb.dataset.url;
                    const filename = cb.dataset.name;
                    
                    // 更新进度提示
                    progress.innerText = `Fetching ${count + 1}/${total}: ${filename}`;
                    
                    // 获取文件二进制数据
                    const response = await fetch(url);
                    if (!response.ok) throw new Error(`Failed to fetch ${url}`);
                    const blob = await response.blob();
                    
                    // 添加到 zip (这里简单处理，所有文件都放在根目录，防止重名可加前缀)
                    // 如果想保留文件夹结构，需要更复杂的 data-path 逻辑
                    zip.file(filename, blob);
                    
                    count++;
                }

                progress.innerText = "Compressing...";
                const content = await zip.generateAsync({type: "blob"});
                
                // 触发下载
                saveAs(content, "PKU_Course_Materials.zip");
                progress.innerText = "Done!";
                
            } catch (err) {
                console.error(err);
                alert("Download failed: " + err.message);
                progress.innerText = "Error!";
            } finally {
                btn.disabled = false;
                updateBtnState();
            }
        }

        // 初始化：根节点自动作为 file-tree 的子级
        const rootUl = document.getElementById('file-tree');
        // 为了让根目录（Computer Science）也是可折叠的 li，我们需要手动包裹一下
        createTree(treeData, rootUl);
        // 默认让顶层目录展开
        const topLevel = rootUl.querySelectorAll('#file-tree > ul > li');
        topLevel.forEach(li => li.classList.add('expanded'));
        
    </script>
</body>
</html>
"""

# 写入文件
with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(html_template.replace("__DATA_JSON__", json.dumps(full_data)))

print("Pro Max Index generated!")