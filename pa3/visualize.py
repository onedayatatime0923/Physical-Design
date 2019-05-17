
import sys, re
import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
from pyqtgraph.dockarea import DockArea, Dock
from collections import defaultdict
import argparse
import matplotlib
from functools import partial
assert QtCore
app = QtGui.QApplication([])

layerColor = list(map(matplotlib.colors.to_rgb, ['blue', 'grey', 'red', 'white', 'green', 'grey', 'orange', 'white', 'purple', 'grey', 'cyan', 'white', 'red', 'grey', 'green', 'white', 'blue', 'grey', 'red', 'white', 'green', 'grey']))
layerColor = list(map(lambda x:(int(x[1][0]*255),int(x[1][1]*255),int(x[1][2]*255),90+40*(x[0]//5)), enumerate(layerColor)))

parser = argparse.ArgumentParser(description='NTUidRoute visualization')
parser.add_argument('input', type=str, help="input file.")
parser.add_argument('-bus', type=str, nargs = "+", default = [], help="bus name.")
opt = parser.parse_args()
inputfile = opt.input
title = "NTUidRoute"
visClass = {
    'layout':        (True, 3, "x"),
}

class MainWindow (QtGui.QMainWindow):
    def __init__ (self):
        super(MainWindow, self).__init__()
        self.layerC = 1
        self.layerObjList = [[] for i in range(self.layerC + 1)]
        self.typeObjList = {}
        self.busNames = []
        self.busObjList= defaultdict(list)
        self.totalObj, self.totalObjC, self.totalObjInfo = [], [], []
        self.typeAction = []
        self.layerAction = []
        self.busAction = []
        #  tmp
        self.tmpObjName = ''
        self.tmpBusName = ''
    def resizeLayer(self, layerC):
        self.layerObjList.extend([[] for i in range(layerC - self.layerC)])
        self.pointList.extend([[[], []] for i in range(layerC - self.layerC)])
        self.lineList.extend([[[], []] for i in range(layerC - self.layerC)])
        self.rectList.extend([[] for i in range(layerC - self.layerC)])
        self.layerC = layerC
    def start(self):
        self.win = QtGui.QMainWindow()
        self.win.resize(1000,500)
        self.win.setWindowTitle(title)
        self.area = DockArea()
        self.visualizeDock = Dock("Visualization", size=(800,500))
        self.visualizePlt = pg.PlotWidget(background=(255,255,255))
        self.visualizePlt.setTitle(title = opt.input, color = (0,0,0))
        self.visualizePlt.showGrid(x = True, y = True)
        self.visualizePlt.getAxis("left").setPen((0,0,0))
        self.visualizePlt.getAxis("top").setPen((0,0,0))
        self.visualizePlt.getAxis("bottom").setPen((0,0,0))
        self.visualizePlt.getAxis("right").setPen((0,0,0))
        self.buttonDock = Dock("Buttons", size=(100,500))
        self.bottonScroll = QtGui.QScrollArea()
        self.buttonGroup = QtGui.QGroupBox()
        self.bottonVBox = QtGui.QVBoxLayout()
    def end(self):
        self.buttonGroup.setLayout(self.bottonVBox)
        self.bottonScroll.setWidget(self.buttonGroup)
        self.buttonDock.addWidget(self.bottonScroll)
        self.visualizeDock.addWidget(self.visualizePlt)
        self.area.addDock(self.buttonDock)
        self.area.addDock(self.visualizeDock, 'left', self.buttonDock)
        self.win.setCentralWidget(self.area)
        self.win.show()
    def beginObj(self, name):
        self.typeObjList[name] = []
        self.tmpObjName = name
    def beginBus(self, name):
        self.tmpBusName = name
        self.pointList = [[[], []] for i in range(self.layerC + 1)]
        self.lineList = [[[], []] for i in range(self.layerC + 1)]
        self.rectList = [[] for i in range(self.layerC + 1)]
    def addPoint(self, x, y ,z):
        self.pointList[z][0].append(x)
        self.pointList[z][1].append(y)
    def addLine(self, x0, y0, z0, x1, y1, z1):
        self.lineList[z0][0].extend([x0, x1])
        self.lineList[z0][1].extend([y0, y1])
    def addRect(self, x0, y0, x1, y1, z):
        self.rectList[z].append((x0, y0, x1-x0, y1-y0))
    def endBus(self, lineWidth, symbol):
        self.addObjs(lineWidth, symbol)
    def addObjs(self, lineWidth, symbol):
        pen = pg.mkPen((0,0,0), width=lineWidth)
        brushes = [pg.mkBrush(color) for color in layerColor]
        for z, (xs, ys) in enumerate(self.pointList):
            if len(xs) > 0:
                scatter = pg.ScatterPlotItem(size=15, pen=None, symbol=symbol, symbolBrush=brushes[z-1])
                scatter.addPoints(xs, ys)
                self.addObj(scatter, z)
        for z, lines in enumerate(self.lineList):
            if len(lines[0]) > 0:
                connect = np.ones(len(lines[0]), dtype=np.ubyte)
                connect[1::2] = 0
                line = pg.arrayToQPath(np.array(lines[0]), np.array(lines[1]), connect)
                line = QtGui.QGraphicsPathItem(line)
                line.setPen(pen)
                self.addObj(line, z)
                self.totalObjInfo.append((self.line2rect(lines), z, self.tmpBusName))
        for z, rects in enumerate(self.rectList):
            if len(rects) > 0:
                obj = QtGui.QGraphicsItemGroup()
                for i, rect in enumerate(rects):
                    rect = QtGui.QGraphicsRectItem(*rect)
                    rect.setPen(pen)
                    rect.setBrush(brushes[z-1])
                    obj.addToGroup(rect)
                obj.mouseDoubleClickEvent = self.printInfo
                self.addObj(obj, z)
                self.totalObjInfo.append((rects, z, self.tmpBusName))
    def addObj(self, obj, z):
        obj.setZValue(z)
        self.visualizePlt.addItem(obj)
        self.layerObjList[z].append((obj, len(self.totalObj)))
        self.typeObjList[self.tmpObjName].append((obj, len(self.totalObj)))
        self.busObjList[self.tmpBusName].append((obj, len(self.totalObj)))
        self.totalObj.append((obj, 3))
        self.totalObjC.append(3)
    def line2rect(self, ls):
        rtn = []
        for i in range(0, len(ls[0]), 2):
            rtn.append((ls[0][i], ls[1][i], ls[0][i+1]-ls[0][i], ls[1][i+1]-ls[1][i]))
        return rtn
    def printInfo(self, event):
        x = event.lastPos().x()
        y = event.lastPos().y()
        clicked_infos = []
        for i in range(len(self.totalObjInfo)):
            if self.totalObjC[i] != self.totalObj[i][1]:
                continue
            infos, z, busName = self.totalObjInfo[i]
            for info in infos:
                if info[0] <= x <= info[0]+info[2] and info[1] <= y <= info[1]+info[3]:
                    name = "{}".format(busName)
                    xyz = "({:d}, {:d}, {:d}), ({:d}, {:d}, {:d})".format(info[0], info[1], z, info[0]+info[2], info[1]+info[3], z)
                    wh = "width = {}, height = {}".format(info[2], info[3])
                    clicked_infos.append("\n".join(["-"*30, xyz, wh, name]))
        msg = QtGui.QMessageBox( QtGui.QMessageBox.Information,
            "Informations",
            "clicked on {}, {}\n".format(int(x), int(y)) + "\n".join(clicked_infos))
        msg.exec_()
    def sortByNum(self, y):
        return re.sub('\d+', lambda m:m.group().zfill(9), y)
    def resetAction(self, action, st):
        action.blockSignals(True)
        action.setChecked(st)
        action.blockSignals(False)
    def updateSt(self, st, objs):
        for obj, i in objs:
            if st:
                self.totalObjC[i] += 1
                if self.totalObjC[i] == self.totalObj[i][1]: obj.show()
            else:
                self.totalObjC[i] -= 1
                if self.totalObjC[i] != self.totalObj[i][1]: obj.hide()
    def typeFunc(self, st, label):
        self.updateSt(st, self.typeObjList[label])
    def totalLayerFunc(self, st):
        for i in range(len(self.layerAction)):
            if self.layerAction[i].isChecked() != st:
                self.updateSt(st, self.layerObjList[i+1])
                self.resetAction(self.layerAction[i], st)
    def layerFunc(self, st, idx):
        self.updateSt(st, self.layerObjList[idx])
    def totalBusFunc(self, st):
        for i in range(len(self.busNames)):
            if self.busAction[i].isChecked() != st:
                self.updateSt(st, self.busObjList[self.busNames[i]])
                self.resetAction(self.busAction[i], st)
    def busFunc(self, st, idx):
        self.updateSt(st, self.busObjList[self.busNames[idx]])
    def typeMenu(self):
        typyM = QtGui.QMenu()
        for typeName, condition in visClass.items():
            if condition[0]:
                self.typeAction.append(typyM.addAction(typeName))
                self.typeAction[-1].setCheckable(True)
                self.typeAction[-1].setChecked(True)
                self.typeAction[-1].toggled.connect(partial(self.typeFunc, label=typeName))
        self.bottonVBox.addWidget(typyM)
    def layerMenu(self):
        totalLayerM = QtGui.QMenu()
        self.totalBusAction = totalLayerM.addAction("Total Layer")
        self.totalBusAction.setCheckable(True)
        self.totalBusAction.setChecked(True)
        self.totalBusAction.toggled.connect(self.totalLayerFunc)
        self.bottonVBox.addWidget(totalLayerM)
        layerM = QtGui.QMenu()
        for i in range(1, self.layerC + 1):
            if(i%2 == 1): self.layerAction.append(layerM.addAction('MetalLayer'+str((i+1)//2)))
            else: self.layerAction.append(layerM.addAction('ViaLayer'+str(i//2)))
            self.layerAction[-1].setCheckable(True)
            self.layerAction[-1].setChecked(True)
            self.layerAction[-1].toggled.connect(partial(self.layerFunc, idx=i))
        layerButton = QtGui.QPushButton("Layers")
        layerButton.setMenu(layerM)
        self.bottonVBox.addWidget(layerButton)
    def busMenu(self):
        self.busNames = list(sorted(self.busObjList.keys(), key=self.sortByNum))
        totalBusM = QtGui.QMenu()
        self.totalBusAction = totalBusM.addAction("Total Bus")
        self.totalBusAction.setCheckable(True)
        self.totalBusAction.setChecked(True)
        self.totalBusAction.toggled.connect(self.totalBusFunc)
        self.bottonVBox.addWidget(totalBusM)
        for i, busName in enumerate(self.busNames):
            busM = QtGui.QMenu()
            self.busAction.append(busM.addAction(busName))
            self.busAction[-1].setCheckable(True)
            self.busAction[-1].setChecked(True)
            self.busAction[-1].toggled.connect(partial(self.busFunc, idx=i))
            font = busM.font()
            font.setPointSize(9)
            busM.setFont(font)
            self.bottonVBox.addWidget(busM)




if __name__ == '__main__':
    mainWindow = MainWindow()

    mainWindow.start()

    for typeName, condition in visClass.items():
        if not condition[0]:  continue
        mainWindow.beginObj(typeName)
        for line in open(inputfile + '.' + typeName, 'r'):
            content = line.split()
            if (len(content) <= 2 and content[0] != '(' and content[0] != ')'):
                if (len(opt.bus) == 0 or content[0] in opt.bus):
                    mainWindow.beginBus(content[0])
                    busStart = True
                else:
                    busStart = False
            elif content[0] == '(':  continue
            elif content[0] == ')': 
                if busStart:
                    mainWindow.endBus(condition[1], condition[2])
            else:
                if busStart:
                    assert len(content) == 6 or len(content) == 3
                    if len(content) == 6:
                        x0, y0, z0, x1, y1, z1 = map(int, content)
                    else:
                        x0, y0, z0 = map(int, content)
                        x1, y1, z1 = x0, y0, z0
                    if max(z0, z1) > mainWindow.layerC: mainWindow.resizeLayer(max(z0,z1))

                    if len(content) == 3:
                        mainWindow.addPoint(x0, y0, z0)
                    elif len(content) == 6 and x0 == x1 and y0 == y1:
                        mainWindow.addPoint(x0, y0, z0)
                        mainWindow.addPoint(x1, y1, z1)
                    elif x0 == x1 or y0 == y1:
                        mainWindow.addLine(x0, y0, z0, x1, y1, z1)
                    else:
                        mainWindow.addRect(x0, y0, x1, y1, z0)

    mainWindow.typeMenu()
    mainWindow.layerMenu()
    mainWindow.busMenu()



    mainWindow.end()
    assert (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION')
    QtGui.QApplication.exec_()


