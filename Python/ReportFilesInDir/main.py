"""
Task: Generating a report of the files in a directory.
Steps:
    * Determine the directory that you want to generate the report for.
    * Get a list of all the files in the directory.
    * For each file, determine the size and modification date.
    * Create a report with the file name, size, and modification date for each file.
Tools and resources:
    * A programming language that can access the file system and read file properties -> Python.
    * A library or framework that can be used to generate a report in a desired format, such as CSV or PDF.
"""

import os
from tabulate import tabulate
from datetime import datetime
import csv
import math

from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.platypus import SimpleDocTemplate, Table, Paragraph, Spacer


def WhatDirectory() -> str:
    # Ask the user to enter the path of the directory
    directory = input("Enter the path of the directory: ")

    while not os.path.isdir(directory):
        directory = input("Please enter a valid directory path: ")

    return directory


def GetListOfFiles(directory) -> list[str]:
    # Get a list of all the files in the specified directory
    files = os.listdir(directory)

    # Return the list of files
    return files


def ForFileIn(files, data:list, directory:str) -> None:
    # Iterate through the list of files
    for file in files:
        # Get the full path of the file
        file_path = os.path.join(directory, file)

        # Check if the path is a directory or file
        if os.path.isdir(file_path):
            # Get the folder name
            file_name = os.path.splitext(file)[0]

            # Size of the dir
            file_size = 0
            for root, dirs, files_in_dir in os.walk(file_path):
                for file_in_dir in files_in_dir:
                    file_path_in_dir = os.path.join(root, file_in_dir)
                    file_size += os.path.getsize(file_path_in_dir)
            
            # No extension then folder
            file_extension = 'FOLDER'
        else:
            # Get size of the file
            file_size = os.path.getsize(file_path)
            # Get extension of the file
            file_extension = os.path.splitext(file)[1][1::]
            # Get the file name
            file_name = os.path.splitext(file)[0]

        # Get the creation and modification date of the file
        file_created_date = os.path.getctime(file_path)
        file_modified_date = os.path.getmtime(file_path)

        # Convert the creation and  modification date from a timestamp to a more readable format
        file_modified_date = datetime.fromtimestamp(file_modified_date).strftime("%Y-%m-%d")
        file_created_date = datetime.fromtimestamp(file_created_date).strftime("%Y-%m-%d")


        # Determine the size unit (KB, MB, GB, etc.) based on the file size
        order_of_magnitude = int(math.log(file_size, 1024)) if file_size > 0 else 0
        size_units = {0: 'bytes', 1: 'KB', 2: 'MB', 3: 'GB'} # Easy to add more
        file_size_unit = size_units[order_of_magnitude]

        file_size = file_size / (1024 ** order_of_magnitude)

        # Add the file name, size, modification date, creation date, and file type to the data list
        data.append([file_name, file_extension, f"{file_size:.4} {file_size_unit}", file_created_date, file_modified_date])



def GenerateReportInConsole(directory:str, files:list[str]) -> None:
    # Create a list to store the data for each file
    data = []

    ForFileIn(files, data, directory)

    # Use tabulate to generate a table of the data
    print(tabulate(data, headers=["Name", "Extension", "Size", "Created", "Modified"], tablefmt="grid"))


def GenerateReportInPDF(directory:str, files:list[str]) -> None:
    # Create a PDF document with landscape orientation and the letter page size
    doc = SimpleDocTemplate("report.pdf", pagesize=letter, rightMargin=30, leftMargin=30, topMargin=30, bottomMargin=18)
    styles = getSampleStyleSheet()

    # Create a list to store the data for each file
    data = []

    # Add the column headers to the data list
    data.append(["Name", "Extension", "Size", "Created", "Modified"])

    ForFileIn(files, data, directory)
    
    # Create a table using the data list and add it to the PDF document
    table = Table(data)

    # Create a paragraph for the title and the text stating the directory analyzed
    style_h1_center = styles['Heading1']
    style_h1_center.alignment = 1  # centered
    title = Paragraph("Directory Analysis Report", style_h1_center)
    style_normal_center = styles["Normal"]
    style_normal_center.alignment = 1
    directory_text = Paragraph("Directory analyzed: " + directory, style_normal_center)
    space = Spacer(1, 12)

    # Add the title, the text stating the directory analyzed, and the table to the PDF document
    doc.build([title, directory_text, space, space, table])


def GenerateReportInCSV(directory:str, files:list[str]) -> None:
    # Create a list to store the data for each file
    data = []
    ForFileIn(files, data, directory)

    # Create the CSV
    with open(f'{directory}/report.csv', 'w', newline='') as csvfile:
        # Create a CSV writer object
        writer = csv.writer(csvfile)

        # Write the column headers
        writer.writerow(['Name', 'Extension', 'Size', 'Created', 'Modified'])

        # Write a row to the CSV file
        for file in data:
            name, extension, size, created, modified = file
            writer.writerow([name, extension, size, created, modified])


if __name__ == '__main__':
    # Get the directory
    directory = WhatDirectory()
    # Get the files and folders
    files = GetListOfFiles(directory)

    end_program = False

    while not end_program:
        action = input("Write 'csv', 'pdf', 'console', or 'exit' to generate the report or exit: ")
        action = action.lower()

        if action == 'csv':
            # Confirmation of valid directory
            print("Generating report for directory:", directory)
            GenerateReportInCSV(directory, files)  # CSV output
        elif action == 'pdf':
            # Confirmation of valid directory
            print("Generating report for directory:", directory)
            GenerateReportInPDF(directory, files)  # PDF output
        elif action == 'console':
            # Confirmation of valid directory
            print("Generating report for directory:", directory)
            GenerateReportInConsole(directory, files)  # Console output
        elif action == 'exit':
            end_program = True

