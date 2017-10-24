# -*- coding: utf-8 -*-
"""
"""
import os
import cv2
import numpy as np
from multiprocessing.dummy import Pool as ThreadPool

NUM_THREAD = 8
def read2chw(filename, w=224, h=224):
    im = cv2.imread(filename)#h,w,c
    im = cv2.resize(im, (w,h))
    im = im.transpose((2, 0, 1))#c,h,w
    return im


def read2nchw(filename, w=224, h=224):
    return np.expand_dims(read2chw(filename), axis=0)


def read2nchw(file_list, w=224, h=224):
    datum = np.empty((batch_size, 3,  data_w, data_h))
    pool = ThreadPool(NUM_THREAD)
    X = pool.map(read2chw, file_list)
    pool.close()
    pool.join()
    for idx, file_ in enumerate(file_list):
        datum[idx,:,:,:] = X[idx][:,:,:]
    return datum


if __name__ == '__main__':
    pass
