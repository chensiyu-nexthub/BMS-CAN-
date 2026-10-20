# -*- coding: utf-8 -*-
from docx import Document
from docx.shared import Pt, Cm
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml.ns import qn

SOURCE = r"D:\xianmu\baogao\本科毕业论文开题报告_最终合并版.docx"
OUTPUT = r"D:/xianmu/BMS-main/本科毕业论文开题报告_提示词模板_学校五项严格原样重排版.docx"

src = Document(SOURCE)
paras = [p for p in src.paragraphs if p.text.strip()]
texts = [p.text for p in paras]

new = Document()

for section in new.sections:
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(2.5)
    section.bottom_margin = Cm(2.5)
    section.left_margin = Cm(3)
    section.right_margin = Cm(2.5)

style = new.styles["Normal"]
style.font.name = "宋体"
style._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
style.font.size = Pt(12)

p = new.add_paragraph()
p.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = p.add_run("本科毕业论文（设计）开题报告 AI 撰写提示词（学校模板严格原样重排版）")
r.bold = True
r.font.size = Pt(16)
r.font.name = "宋体"
r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")

# 索引基于原文档非空段落，0起。
# 1-108原文索引见最终合并版。
def add_original(index):
    text = texts[index]
    p = new.add_paragraph()
    r = p.add_run(text)
    r.font.size = Pt(12)
    r.font.name = "宋体"
    r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    p.paragraph_format.line_spacing = Pt(20)
    if text.startswith("一、") or text.startswith("二、") or text.startswith("三、") or text.startswith("四、") or text.startswith("五、") or text.startswith("六、") or text.startswith("七、"):
        r.bold = True
        r.font.size = Pt(14)
        p.paragraph_format.space_before = Pt(12)
        p.paragraph_format.space_after = Pt(6)
    elif text in {"研究目的：", "研究意义：", "国内研究现状", "国外研究现状（有两个版本，自选）", "主要内容：", "预期目标：", "拟解决关键问题：", "研究方法：", "技术路线：", "五、主要参考文献", "六、进度安排", "七、大纲"}:
        r.bold = True
        p.paragraph_format.space_before = Pt(6)
    elif text.startswith("版本一") or text.startswith("版本二") or text.startswith("1.") or text.startswith("2.") or text.startswith("3."):
        r.bold = True
        p.paragraph_format.space_before = Pt(4)
    return p

def add_school_heading(text):
    p = new.add_paragraph()
    r = p.add_run(text)
    r.bold = True
    r.font.size = Pt(14)
    r.font.name = "宋体"
    r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    p.paragraph_format.space_before = Pt(12)
    p.paragraph_format.space_after = Pt(6)
    p.paragraph_format.line_spacing = Pt(20)
    return p

def add_note(text):
    p = new.add_paragraph()
    r = p.add_run(text)
    r.italic = True
    r.font.size = Pt(10.5)
    r.font.name = "宋体"
    r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    p.paragraph_format.line_spacing = Pt(18)
    p.paragraph_format.space_after = Pt(6)
    return p

# 说明：仅分组位置调整，原文内容不改写。
p = new.add_paragraph()
r = p.add_run("说明：本文件仅按学校模板五项调整提示词分组位置。以下所有正文内容均从原最终合并版原样复制，没有修改、压缩或改写。")
r.font.size = Pt(12)
r.font.name = "宋体"
r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
p.paragraph_format.line_spacing = Pt(20)
p.paragraph_format.space_after = Pt(12)

# 一、综述本课题国内外研究动态，说明选题的依据和意义
add_school_heading("一、综述本课题国内外研究动态，说明选题的依据和意义")
add_note("原研究目的、研究意义、国内外研究现状均归入此处。")
# 原文3-108中的：研究目的和意义（3-9）；国内现状（10-35）；国外现状（36-85）
for i in range(2, 85):
    add_original(i)

# 二、研究的基本内容，拟解决的主要问题
add_school_heading("二、研究的基本内容，拟解决的主要问题")
add_note("原主要内容、预期目标、拟解决关键问题归入此处。")
for i in range(85, 95):
    add_original(i)

# 三、研究步骤、方法及措施
add_school_heading("三、研究步骤、方法及措施")
add_note("原研究方法与技术路线归入此处。")
for i in range(95, 102):
    add_original(i)

# 四、研究工作进度
add_school_heading("四、研究工作进度")
add_note("原进度安排归入此处。")
for i in range(104, 106):
    add_original(i)

# 五、主要参考文献
add_school_heading("五、主要参考文献")
add_note("原主要参考文献归入此处。")
for i in range(102, 104):
    add_original(i)

# 附加：大纲
add_school_heading("附加、大纲（不属于学校模板五项，需要时单独使用）")
add_note("原大纲归入此处。")
for i in range(106, 108):
    add_original(i)

# 原结尾说明
add_original(1)
for i in range(108, len(texts)):
    add_original(i)

new.save(OUTPUT)
print(OUTPUT)
