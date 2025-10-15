import tkinter as tk
from tkinter import colorchooser, messagebox
from PIL import Image

GRID_SIZE = 16  # favicon is 16x16
PIXEL_SIZE = 20  # display scale

class FaviconApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Favicon Drawer 🎨")

        self.canvas = tk.Canvas(root, width=GRID_SIZE*PIXEL_SIZE, height=GRID_SIZE*PIXEL_SIZE, bg="white")
        self.canvas.pack(padx=10, pady=10)

        self.color = "#000000"
        self.pixels = [[None for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]

        # Draw grid
        for y in range(GRID_SIZE):
            for x in range(GRID_SIZE):
                rect = self.canvas.create_rectangle(
                    x*PIXEL_SIZE, y*PIXEL_SIZE,
                    (x+1)*PIXEL_SIZE, (y+1)*PIXEL_SIZE,
                    fill="white", outline="gray"
                )
                self.pixels[y][x] = rect

        # Buttons
        btn_frame = tk.Frame(root)
        btn_frame.pack(pady=5)

        tk.Button(btn_frame, text="🎨 Pick Color", command=self.pick_color).grid(row=0, column=0, padx=5)
        tk.Button(btn_frame, text="💾 Save Favicon", command=self.save_icon).grid(row=0, column=1, padx=5)
        tk.Button(btn_frame, text="🧹 Clear", command=self.clear).grid(row=0, column=2, padx=5)

        self.canvas.bind("<Button-1>", self.paint)

    def pick_color(self):
        color = colorchooser.askcolor(title="Choose Color")
        if color[1]:
            self.color = color[1]

    def paint(self, event):
        x = event.x // PIXEL_SIZE
        y = event.y // PIXEL_SIZE
        if 0 <= x < GRID_SIZE and 0 <= y < GRID_SIZE:
            self.canvas.itemconfig(self.pixels[y][x], fill=self.color)

    def clear(self):
        for y in range(GRID_SIZE):
            for x in range(GRID_SIZE):
                self.canvas.itemconfig(self.pixels[y][x], fill="white")

    def save_icon(self):
        img = Image.new("RGBA", (GRID_SIZE, GRID_SIZE), (255, 255, 255, 0))
        for y in range(GRID_SIZE):
            for x in range(GRID_SIZE):
                color = self.canvas.itemcget(self.pixels[y][x], "fill")
                if color != "white":
                    img.putpixel((x, y), tuple(int(color[i:i+2], 16) for i in (1, 3, 5)) + (255,))
        img.save("favicon.ico", format="ICO")
        messagebox.showinfo("Saved!", "✅ favicon.ico has been saved!")

root = tk.Tk()
app = FaviconApp(root)
root.mainloop()
