# -*- coding: utf-8 -*-
"""
Created on Wed Jan 20 10:46:17 2016

@author: crw
"""
import os
import numpy as np
from model import Model
from PIL import Image
import cPickle


def evaluate(root_path, result_path):
    model = Model("/home/huaijing/slam/PSPNet/evaluation/prototxt/pspnet50_ADE20K_473.prototxt",
                  "/home/huaijing/slam/PSPNet/evaluation/model/pspnet50_ADE20K.caffemodel",
                  gpu_id = 3,
                  output_layers=["conv6_interp"],
                  means=[104, 117, 124],
            )

    cmap = None
    cmap_path = './cmap.pkl'
    with open(cmap_path) as f:
        cmap = cPickle.load(f)

    for idx, filename in enumerate(os.listdir(root_path)):
        resultname = os.path.join(result_path, filename)
        resultname = resultname[:-4] + ".png"
        filename = os.path.join(root_path, filename)
        result = model.predict(filename, 473, 473)[0]
        print(result.shape)
        print("idx: ",idx, np.argmax(result,0).shape)
        tt = np.argmax(result, 0)
        print(tt)
        print(tt.shape)
        print(np.sum(tt))

        tt[tt < 3] = 0
        tt[tt > 3] = 0

        im_to_save = Image.fromarray(tt.astype(np.uint8))
        im_to_save.putpalette(cmap.ravel())
        print(resultname)
        im_to_save.save(resultname)


if __name__ == '__main__':
    path = "../data/images/cam/fill/"
    rpath = "../data/images/cam_result/"
    evaluate(path, rpath)
