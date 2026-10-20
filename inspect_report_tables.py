from docx import Document

p = r"C:\Users\chens\Desktop\linshi\桌面补充图片检查.docx"
d = Document(p)
for ti, table in enumerate(d.tables):
    print("TABLE", ti, "ROWS", len(table.rows), "COLS", len(table.columns))
    for ri, row in enumerate(table.rows):
        cells = [c.text.replace("\r", " ").replace("\n", " ") for c in row.cells]
        print(ri, repr(cells))
