from fpdf import FPDF
from PyPDF2 import PdfReader, PdfWriter

# Step 1: Create a basic PDF
pdf = FPDF()
pdf.add_page()
pdf.set_font("Arial", size=16)
pdf.cell(200, 10, txt="This PDF has JS!", ln=True, align="C")
pdf.output("temp.pdf")

# Step 2: Embed JavaScript
reader = PdfReader("temp.pdf")
writer = PdfWriter()

# Copy pages
for page in reader.pages:
    writer.add_page(page)

# Add JavaScript: show an alert on open
writer.add_js("app.alert('Hello World from PDF JS!');")

# Write the final PDF
with open("hello_js.pdf", "wb") as f:
    writer.write(f)

print("PDF with JS created: hello_js.pdf")
