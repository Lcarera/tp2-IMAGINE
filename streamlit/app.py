import streamlit as st
import pandas as pd
import imageio
import numpy as np
from PIL import Image
from io import BytesIO
import matplotlib.pyplot as plt
import pickle
import os
import cv2
from time import sleep
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)

# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')

def show_image():
    img = imageio.imread('../src/out/prueba.ppm')
    plt.imshow(img)
    st.pyplot()

def run_filters(z, b, c1, c2, m, s, n, blur, brightness, crop, merge, shades, zoom):
    filters = ''
    parametersOne = ''
    parametersTwo = ''
    threads = f'{n} '
    if blur: 
        filters += 'boxblur '
        parametersOne += '0 '
        parametersTwo += '0 '
    if brightness:
        filters += 'brightness '
        parametersOne += f'{b} '
        parametersTwo += '0 '
    if crop:
        filters += 'crop '
        parametersOne += f'{c1} '
        parametersTwo += f'{c2} '
    if merge:
        filters += 'merge '
        parametersOne += f'{m} '
        parametersTwo += '0 '
    if shades:
        filters += 'shades '
        parametersOne += f'{s} '
        parametersTwo += '0 '
    if zoom:
        filters += 'zoom '
        parametersOne += f'{z} '
        parametersTwo += '0 '

    filters = filters[0:-1]
    parametersOne = parametersOne[0:-1]
    parametersTwo = parametersTwo[0:-1]
    cmd = '../src/main '  + '"'+ filters + '" '+ threads + '"'+ parametersOne + '" ' + '../src/imgs/ashitaka.ppm ../src/out/prueba.ppm ' + '"'+ parametersTwo + '" ' + '0'
    os.system(cmd)

# Sidebar
st.sidebar.header('Parámetros')

blur = st.sidebar.checkbox('Box blur',value=False)
brightness = st.sidebar.checkbox('Brightness',value=False)
crop = st.sidebar.checkbox('Crop',value=False)
merge = st.sidebar.checkbox('Merge',value=False)
shades = st.sidebar.checkbox('Shades',value=False)
zoom = st.sidebar.checkbox('Zoom',value=False)
z = None
b = None
c1 = None
c2 = None
m = None
s = None

if brightness:
    b = st.sidebar.slider('Brillo', -100, 100, 0, 1, '%d')/100
if crop:
    c1 = st.sidebar.slider('Vertical',100, 500, 0, 1, '%d')
    c2 = st.sidebar.slider('Horizontal',100, 500, 0, 1, '%d')
if merge:
    m = st.sidebar.slider('Cantidad de merge', 1, 100, 0, 1, '%d')/100
if shades:
    s = st.sidebar.slider('Cantidad de sombras', 1, 255, 0, 1, '%d')
if zoom:
    z = st.sidebar.slider('Cantidad de zoom', 1, 4, 0, 1, '%d')

n = st.sidebar.slider('Cantidad de threads', 1, 32, 1, 1, '%d')

if st.sidebar.button('Aplicar'):
    run_filters(z, b, c1, c2, m, s, n, blur, brightness, crop, merge, shades, zoom)
    sleep(3)
    show_image()




