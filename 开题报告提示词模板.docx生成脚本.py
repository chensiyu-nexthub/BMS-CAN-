# -*- coding: utf-8 -*-
from docx import Document
from docx.shared import Pt, Cm
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn

OUTPUT = r"D:\xianmu\baogao\本科毕业论文开题报告_学校模板对齐版.docx"

doc = Document()

# 页面设置：A4，按学校开题报告要求设置页边距
for section in doc.sections:
    section.page_width = Cm(21)
    section.page_height = Cm(29.7)
    section.top_margin = Cm(2.5)
    section.bottom_margin = Cm(2.5)
    section.left_margin = Cm(3)
    section.right_margin = Cm(2.5)

# 正文样式：小四宋体，固定20磅行距
style = doc.styles["Normal"]
style.font.name = "宋体"
style._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
style.font.size = Pt(12)

p = doc.add_paragraph()
p.alignment = WD_ALIGN_PARAGRAPH.CENTER
run = p.add_run("本科毕业设计（论文）开题报告AI撰写提示词（学校模板对齐版）")
run.bold = True
run.font.size = Pt(16)
run.font.name = "宋体"
run._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")

intro = doc.add_paragraph()
intro.add_run("请基于以下提示词模块，撰写开题报告。使用时从每个模块中复制对应提示词，并补充额外要求或文献资料。提示词顺序已按学校空白模板的五个栏目排列。")
intro.paragraph_format.line_spacing = Pt(20)

# 辅助函数
def add_heading(text, level=1):
    h = doc.add_paragraph()
    r = h.add_run(text)
    r.bold = True
    r.font.size = Pt(14 if level == 1 else 12)
    r.font.name = "宋体"
    r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    h.paragraph_format.space_before = Pt(12)
    h.paragraph_format.space_after = Pt(6)
    h.paragraph_format.line_spacing = Pt(20)
    return h

def add_label(text):
    p = doc.add_paragraph()
    r = p.add_run(text)
    r.bold = True
    r.font.size = Pt(12)
    r.font.name = "宋体"
    r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    p.paragraph_format.line_spacing = Pt(20)
    p.paragraph_format.space_before = Pt(4)
    return p

def add_text(text):
    p = doc.add_paragraph()
    r = p.add_run(text)
    r.font.size = Pt(12)
    r.font.name = "宋体"
    r._element.rPr.rFonts.set(qn("w:eastAsia"), "宋体")
    p.paragraph_format.line_spacing = Pt(20)
    return p

COMMON = "写以上内容，正文以段落形式输出，段与段之间字数相差100字以上，表达轻学术化、易读且单句不要过长，段落之间不要用排比句，不要用双引号，全部符号使用中文格式，包括不限于逗号，括号等；中英文之间不要用空格隔开；不要使用“首先，其次，最后，然而、综上所述、此外、具体而言、与此同时”等过渡词。结合上下文，不要有原则性错误以及严重的AI痕迹。"
LIST_COMMON = "写以上内容，正文以分条形式输出，不要超过五条，带标号，表达轻学术化、易读且单句不要过长，段落之间不要用排比句，不要用双引号，全部符号使用中文格式，包括不限于逗号，括号等；中英文之间不要用空格隔开；不要使用“首先，其次，最后，然而、综上所述、此外、具体而言、与此同时”等过渡词。结合上下文，不要有原则性错误以及严重的AI痕迹。"

# 一、综述本课题国内外研究动态，说明选题的依据和意义
add_heading("一、综述本课题国内外研究动态，说明选题的依据和意义")
add_label("选题目的：")
add_text(COMMON)
add_text("额外要求如下：")
add_label("选题意义：")
add_text(COMMON)
add_text("额外要求如下：")
add_label("国内研究动态：")
add_text("论文题目：【此处填入论文题目】")
add_text("输入文献列表（必须严格基于以下内容）：")
add_text("【此处粘贴找好的中文文献列表】")
add_text("引用要求：必须引用全部中文文献，正文中标注对应序号。文中必须明确提及作者名字，例如张三[1]认为或李四等[2]提出，严禁只在句尾标注序号。")
add_text("输出内容：国内研究动态正文。全文以段落形式输出，严禁使用列表或要点符号，相邻段落字数差保持在100字以上，全部使用中文标点，中英文之间不空格，避免使用首先、其次、最后、然而、综上所述、此外等连接词。")
add_label("国外研究动态：")
add_text("版本一：自备文献。请基于稍后提供的外文文献，撰写国外研究动态。引用序号衔接中文文献，正文中明确提及真实第一作者姓氏，严禁只在句尾标注序号。全文以段落形式输出，相邻段落字数差保持在100字以上。")
add_text("版本二：联网检索。请检索与论文题目高度相关的真实外文文献，优先学术期刊论文，确保题名互不相同且来源真实。正文引用序号顺延中文文献，正文中提及真实第一作者姓氏，严禁出现完整文献题名和链接。")
add_text("通用要求：国内外研究动态与选题依据、意义应形成自然衔接，先写研究动态，再引出本课题选题依据和意义，不要机械分点罗列。")

# 二、研究的基本内容，拟解决的主要问题
add_heading("二、研究的基本内容，拟解决的主要问题")
add_label("基本内容：")
add_text(COMMON)
add_text("额外要求如下：")
add_label("预期目标：")
add_text(LIST_COMMON)
add_text("额外要求如下：")
add_label("拟解决的主要问题：")
add_text(LIST_COMMON)
add_text("额外要求如下：")

# 三、研究步骤、方法及措施
add_heading("三、研究步骤、方法及措施")
add_label("研究方法：")
add_text(LIST_COMMON)
add_text("额外要求如下：")
add_label("技术路线：")
add_text(COMMON + "最后以mermaid形式输出一份竖向技术路线图。")
add_text("额外要求如下：")
add_label("保障措施：")
add_text(LIST_COMMON)
add_text("额外要求如下：")

# 四、研究工作进度
add_heading("四、研究工作进度")
add_text("请结合学校要求或指导教师意见，生成研究工作进度。正文以分条形式输出，不要超过八条，每条包含时间阶段和对应工作内容，表达清楚但不夸张。")
add_text("额外要求如下：")

# 五、主要参考文献
add_heading("五、主要参考文献")
add_text("请基于国内外研究动态部分实际引用的文献，整理主要参考文献列表。列表需统一编号，去除DOI和访问地址，按中文文献在前、外文文献在后排列。")
add_text("格式总则：用[1]编号，无小数点。")
add_text("期刊论文格式：[序号]作者.篇名[J].刊名,出版年份,卷(期)号:页码范围.")
add_text("学位论文格式：[序号]作者.论文名[D].学校,年份.")
add_text("教材著作格式：[序号]作者.书名[M].出版地:出版社,出版年份.")
add_text("注意：统一标点符号，检查作者、题名、刊名、年份、卷期和页码信息，严禁编造文献。")

# 附加：论文大纲
add_heading("附加一、论文大纲（不属于学校模板五项，需要时单独使用）")
add_text("请基于本课题生成论文大纲，采用二级标题即可，不要写得太细，方便正式撰写论文时调整。")
add_text("额外要求如下：")

# 使用说明
add_heading("使用说明")
add_text("使用时先复制学校模板对应栏目的提示词，再补充课题题目、文献列表和额外要求。国外研究动态两个版本任选其一。若学校或指导教师另有格式要求，以学校要求为准。")
add_text("以上提示词已按学校模板五部分顺序重排，原模板中的研究目的、研究意义、国内外研究现状、预期目标、研究方法、技术路线和论文大纲均已调整到对应位置。")

OUTPUT = r'D:/xianmu/BMS-main/本科毕业论文开题报告_提示词模板_学校五项对齐版.docx'
doc.save(OUTPUT)
print(OUTPUT)
