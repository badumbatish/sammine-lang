import subprocess
import webbrowser
import os

def generate_docs(doxygen_file):
    # Run Doxygen with the specified Doxyfile
    subprocess.run(['doxygen', doxygen_file])

def open_generated_docs():
    # Open the index.html file in the generated_docs directory
    index_file_path = os.path.join('generated_docs', 'html', 'index.html')
    webbrowser.open(index_file_path)

if __name__ == "__main__":
    # Specify the name of your Doxyfile
    doxygen_file = 'Doxyfile'

    # Generate documentation
    generate_docs(doxygen_file)

    # Open the generated documentation in a web browser
    open_generated_docs()
