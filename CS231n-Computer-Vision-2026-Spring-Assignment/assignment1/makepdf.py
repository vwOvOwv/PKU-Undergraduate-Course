import argparse
import os
import subprocess

try:
    from PyPDF2 import PdfMerger

    MERGE = True
except ImportError:
    print("Could not find PyPDF2. Leaving pdf files unmerged.")
    MERGE = False


def main(files, pdf_name):
    os_args = [
        "jupyter",
        "nbconvert",
        "--log-level",
        "CRITICAL",
        "--to",
        "pdf",
    ]
    for f in files:
        os_args.append(f)
        subprocess.run(os_args)
        os_args.pop()
        print("Created PDF {}.".format(f))
    if MERGE:
        pdfs = [f.split(".")[0] + ".pdf" for f in files]
        merger = PdfMerger()
        for pdf in pdfs:
            merger.append(pdf)
        merger.write(pdf_name)
        merger.close()
        for pdf in pdfs:
            os.remove(pdf)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # We pass in a explicit notebook arg so that we can provide an ordered list
    # and produce an ordered PDF.
    parser.add_argument("--notebooks", type=str, nargs="+", required=True)
    parser.add_argument("--pdf_filename", type=str, required=True)
    args = parser.parse_args()
    main(args.notebooks, args.pdf_filename)
