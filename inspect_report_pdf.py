from pypdf import PdfReader

p = r"C:\Users\chens\Desktop\linshi\桌面补充图片检查(1).pdf"
r = PdfReader(p)
print("PDF_PAGES", len(r.pages))
for i, page in enumerate(r.pages, 1):
    text = page.extract_text() or ""
    lines = [z.strip() for z in text.splitlines() if z.strip()]
    print("PAGE", i)
    for z in lines[:12]:
        print(z)
    print("---")
