import ctypes
from _ctypes import LoadLibrary
import tkinter as tk
from tkinter import Label, messagebox, filedialog, ttk, font
import os


class HuffmanApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Huffman Coding App")

        # Set the dimensions of the window (width x height)
        window_width = 1000
        window_height = 800

        # Get the screen width and height
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()

        # Calculate the position to center the window
        x_position = (screen_width // 2) - (window_width // 2)
        y_position = (screen_height // 2) - (window_height // 2)

        # Set the dimensions and position of the window
        root.geometry(f"{window_width}x{window_height}+{x_position}+{y_position}")

        root.grid_columnconfigure(0, weight=1)

        self.filepath = ""
        self.initial_file_size = 0

        # Create frame
        self.header_frame = tk.Frame(root, bg='#5C5C5C')
        self.header_frame.grid(row=0, sticky='nsew')  # Use grid to place the frame

        self.header_frame.grid_columnconfigure(0, weight=1)
        self.header_frame.grid_rowconfigure(0, weight=1)

        custom_font = font.Font(family="Helvetica", size=14, weight="bold")

         # Create button to browse files
        self.browse_button = tk.Button(self.header_frame, text="Browse Files", command=self.browseFiles, background="#A59E8C", font=custom_font)
        self.browse_button.grid(row=0, column=0, pady=10)  # Place the button to browse files

        # Create frame
        self.frame = tk.Frame(root)
        self.frame.grid(row=1, sticky='nsew')  # Use grid to place the frame

        self.frame.grid_columnconfigure(0, weight=1)
        self.frame.grid_rowconfigure(0, weight=1)

        # Create and place the label for file explorer
        self.label_file_explorer = Label(self.frame, width=100, height=2, fg="#19F7FF", font=custom_font)
        self.label_file_explorer.grid(row=0)

        # Create and place the label for file explorer
        self.label_initial_file_size = Label(self.frame, width=100, height=2, fg ="#19F7FF", font=custom_font)
        self.label_initial_file_size.grid(row=1)

        # Create compress button
        self.compress_button = tk.Button(self.frame, text="Compress", command=self.compress, background='#19F7FF', height=1, width=14, font=custom_font)
        self.compress_button.grid(row=2, pady=40)

        # Create decompress button
        self.decompress_button = tk.Button(self.frame, text="Decompress", command=self.decompress, background='#19F7FF', height=1, width=14, font=custom_font)
        self.decompress_button.grid(row=3, pady=40)

        # Create progress bar
        self.progress_bar = tk.ttk.Progressbar(self.frame, orient='horizontal', length=400, mode='determinate', maximum=1000)
        self.progress_bar.grid(row=4, pady=60)
        self.label_progress = Label(self.frame, width=100, height=2, fg ="#19F7FF", font=custom_font)
        self.label_progress.grid(row=5)

        # Create and place the label for file explorer
        self.label_compressed_file_size = Label(self.frame, width=100, height=2, fg ="#19F7FF", font=custom_font)
        self.label_compressed_file_size.grid(row=6)

        # Create and place the label for file explorer
        self.label_compression_amount = Label(self.frame, width=100, height=2, fg ="#19F7FF", font=custom_font)
        self.label_compression_amount.grid(row=7)

        self.exit_button = tk.Button(root, text="Exit", command=root.destroy, background='#BA1200', width=8, font=custom_font)
        self.exit_button.grid(row=3, pady=15)

    def browseFiles(self):
        self.filepath = filedialog.askopenfilename(initialdir = "/", title = "Select a File", filetypes = (("all files", "*.*"),))
        
        self.label_file_explorer.configure(text="File Opened: " + self.filepath)

        if self.filepath:
            self.initial_file_size = os.path.getsize(self.filepath)  # Get size in bytes
            initial_file_size_kb = self.initial_file_size / 1024  # Convert to KB
            self.label_initial_file_size.configure(text="Initial file size: " + str(round(initial_file_size_kb, 2)) + " KB")

        # Load the shared library
        if ctypes._os.name == 'nt':  # Windows
            self.handle = LoadLibrary('./libhuffman.dll')
            self.lib = ctypes.CDLL(name = self.filepath, handle = self.handle)
        else:  # Linux/macOS
            self.lib = ctypes.CDLL('./libhuffman.so')

        self.ProgressCallback = ctypes.CFUNCTYPE(None, ctypes.c_int)  # Callback type
        self.lib.compress.argtypes = [ctypes.c_char_p, self.ProgressCallback]  # Filepath and callback
        self.lib.compress.restype = None

    def update_progress(self, value):
        self.progress_bar['value'] = value
        percent = int(value / 10)
        self.label_progress.configure(text=str(percent) + '%')
        self.root.update_idletasks()

    def compress(self):
        self.compress_button.config(text="Compressing...")

        progress_callback = self.ProgressCallback(self.update_progress)
        try:
            self.lib.compress(self.filepath.encode('utf-8'), progress_callback) # Call the C++ main function

            self.compress_button.config(text="Compress")

            compressed_file_size = os.path.getsize('compressed.out')  # Get size in bytes
            compressed_file_size_kb = compressed_file_size / 1024  # Convert to KB
            self.label_compressed_file_size.configure(text="Compressed file size: " + str(round(compressed_file_size_kb, 2)) + " KB")

            compressed_amount = ((self.initial_file_size - compressed_file_size) / self.initial_file_size) * 100
            self.label_compression_amount.configure(text="Your file was compressed by " + str(round(compressed_amount, 2)) + "%!")
            
        except Exception as e:
            messagebox.showerror("Error", str(e))
        
    def decompress(self):
        self.decompress_button.config(text="Decompressing...")

        progress_callback = self.ProgressCallback(self.update_progress)
        try:
            self.lib.decompress(progress_callback) # Call the C++ main function

            self.decompress_button.config(text="Decompress")

            decompressed_file_size = os.path.getsize('decompressed.out')  # Get size in bytes
            decompressed_file_size_kb = decompressed_file_size / 1024  # Convert to KB
            self.label_compressed_file_size.configure(text="Decompressed file size: " + str(round(decompressed_file_size_kb, 2)) + " KB")
            self.label_compression_amount.configure(text="");
        except Exception as e:
            messagebox.showerror("Error", str(e))

root = tk.Tk()
root.configure(bg='#4C5760')
huffman = HuffmanApp(root)
root.mainloop()
