from eyed3 import id3
from mutagen import File
from pdfminer.pdfparser import PDFParser
from pdfminer.pdfdocument import PDFDocument
from PIL import Image
import base64
import gnupg
import os
import xml.etree.ElementTree as ET
import zipfile

def main():
    # First key: comment in zip file
    zf = zipfile.ZipFile('container.zip', 'r')
    filename = 'tuenti.docx'
    print zf.getinfo(filename).comment
    
    # Extracting:
    zf.extract(filename)
    
    # Second key: hidden in one of the files of the doc
    zf = zipfile.ZipFile(filename, 'r')
    zf.extractall()
    tree = ET.parse('docProps/core.xml')
    root = tree.getroot()
    print root[1].text
    
    # Third key: Lyrics of the mp3
    tag = id3.Tag()
    tag.parse("tuenti.mp3")
    print "KEY3:", tag.lyrics[0].text[1:]
    
    # Fourth key: title of front cover of the mp3
    f = File('tuenti.mp3')
    artwork = f.tags['APIC:'].data
    with open('tuenti.png', 'wb') as img:
        img.write(artwork)
        
    image = Image.open('tuenti.png')
    print image.info['Title']
    
    # Fifth key: subject of the pdf
    data = base64.b64decode(image.info['NextLevel'])
    with open('tuenti.pdf', 'wb') as pdf:
        pdf.write(data)
    with open('tuenti.pdf', 'rb') as pdf:
        parser = PDFParser(pdf)
        doc = PDFDocument(parser)
        print doc.info[0]['Subject'][3::2]
    
    # Sixth key: subtitles of a video in the pdf
    os.system('pdfdetach -saveall tuenti.pdf 2> /dev/null')
    # The key is given as a subtitle, I guess there is a way to read
    # the image and transform it to plain text, but it will take much
    # longer than just copying it by hand
    print('KEY6: XrBV392qT0DqDSCGuGDStdY6_ADKkD_4')
    
    # Seventh key: first line of uuencoded attached file in the video
    # Sorry, it's late and I'm tired of finding python packages
    os.system('mkvextract attachments video.mkv 1:tuenti.txt > /dev/null')
    with open('tuenti.txt', 'r') as f:
        print f.readline()[:-1]
    
    # Eigth key: title of a file in the uudecoded file
    os.system('uudecode tuenti.txt > /dev/null')
    os.system('gzip -cd ramdisk.cpio.gz | cpio -idmv 2> /dev/null')
    for f in os.listdir('.'):
        if f[:4] == 'KEY8':
            filekey = f
    print filekey
    
    # Ninth key: extract the gpg file using the password from filekey
    with open(filekey, 'r') as f:
        password = f.readline()[12:-1]
    gpg = gnupg.GPG()
    with open('LastLevel.txt.gpg', 'r') as f:
        encrypted_string = str(f.read())
    decrypted_data = gpg.decrypt(encrypted_string, passphrase=password)
    print decrypted_data.data.splitlines()[0]
    
if __name__ == '__main__':
    main()