from pypdfium2 import PdfDocument

p = r"C:\Users\chens\Desktop\linshi\桌面补充图片检查(1).pdf"
pdf = PdfDocument(p)
for i in [1, 10, 11, 12, 13, 14, 15, 16]:
    page = pdf[i]
    bitmap = page.render(scale=1.4)
    image = bitmap.to_pil()
    out = rf"D:\xianmu\BMS-main\report_page_{i+1}.png"
    image.save(out)
    print(out)
