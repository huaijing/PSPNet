# -*- coding: utf-8 -*-
"""
Created on Wed Jan 20 10:46:17 2016

@author: crw
"""
import os
import numpy as np
import sys
caffe_root = '/home/huaijing/slam/PSPNet/'
sys.path.insert(0, caffe_root + 'python')
import caffe
import reader


class Model:
    def __init__(self, deploy_path, model_path, input_layers=['data'], output_layers=['prob'], gpu_id=-1, means=[128,128,128]):
        if gpu_id == -1:
            caffe.set_mode_cpu()
        else:
            caffe.set_mode_gpu()
            caffe.set_device(gpu_id)
        self.deploy_path = deploy_path
        self.model_path = model_path
        self.means = means
        #TODO(crw): support multi input
        assert(len(input_layers) == 1)
        self.input_layers = input_layers
        self.output_layers = output_layers
        self.net = caffe.Classifier(deploy_path, model_path)


    def predict(self, filename, w=224, h=224):
        im_chw = reader.read2chw(filename, w, h).astype(np.float32)
        im_chw[0,:,:] = im_chw[0,:,:] - self.means[0]
        im_chw[1,:,:] = im_chw[1,:,:] - self.means[1]
        im_chw[2,:,:] = im_chw[2,:,:] - self.means[2]
        #im_chw -=np.array(self.means)
        self.net.blobs[self.input_layers[0]].reshape(1,*im_chw.shape)
        self.net.blobs[self.input_layers[0]].data[0] = im_chw
        self.net.forward()
        result = []
        for output_layer in self.output_layers:
            result.append(self.net.blobs[output_layer].data[0])
        return result


if __name__ == '__main__':
    evaluate()
