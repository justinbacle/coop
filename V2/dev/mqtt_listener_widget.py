import sys
import os
import typing
from PyQt5 import QtWidgets, QtCore, QtGui
import paho.mqtt.client as mqtt

sys.path.append(os.getcwd())
from V2.tools import misc  # noqa E402
import V2.config as config  # noqa E402
from V2.dev import modes  # noqa E402


class ViewTree(QtWidgets.QTreeWidget):
    def __init__(self):
        super().__init__()

        # refresh timer
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.refresh)
        self.timer.start(1000)

        # set qtreewidget
        self.setColumnCount(2)
        self.setHeaderLabels(["Sensor", "Data"])

        # set data dict
        self.dataDict = {}

    def fillTree(self, parent, dataDict={}):
        for _k, _v in dataDict.items():
            _node = QtWidgets.QTreeWidgetItem(parent)
            _node.setExpanded(True)
            _node.setText(0, _k)
            if isinstance(_v, dict):
                self.fillTree(_node, _v)
            else:
                _node.setText(1, str(_v))

    def refresh(self):
        self.clear()
        self.fillTree(self, self.dataDict)

    @QtCore.pyqtSlot(dict)
    def updateData(self, dataDict: dict):
        self.dataDict = dict(misc.mergeDicts(self.dataDict, dataDict))
        self.refresh()


class ModeWidget(QtWidgets.QListWidget):
    def __init__(self, parent: typing.Optional[QtWidgets.QWidget] = None) -> None:
        super().__init__(parent=parent)


class ModeItem(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(ModeItem, self).__init__(parent=parent)
        self.textQVBoxLayout = QtWidgets.QVBoxLayout()
        self.modeNameLabel = QtWidgets.QLabel()
        self.modeStatusLabel = QtWidgets.QLabel()
        self.textQVBoxLayout.addWidget(self.modeNameLabel)
        self.textQVBoxLayout.addWidget(self.modeStatusLabel)
        self.allQHBoxLayout = QtWidgets.QHBoxLayout()
        self.modeIdLabel = QtWidgets.QLabel()
        self.allQHBoxLayout.addWidget(self.modeIdLabel, 0)
        self.allQHBoxLayout.addLayout(self.textQVBoxLayout, 1)
        self.setLayout(self.allQHBoxLayout)
        # setStyleSheet
        self.modeNameLabel.setStyleSheet('''
            color: rgb(0, 0, 0);
        ''')
        self.modeStatusLabel.setStyleSheet('''
            color: rgb(50, 50, 50);
        ''')
        self.modeIdLabel.setStyleSheet('''
            color: rgb(0, 0, 0);
            font-weight:200;
            font-size: 26pt;
            margin-right: 12px;
            border: 1px solid black;
            border-radius: 10px;
        ''')

    def setModeName(self, text):
        self.modeNameLabel.setText(text)

    def setModeStatus(self, text):
        self.modeStatusLabel.setText(text)

    def setModeIdLabel(self, text):
        self.modeIdLabel.setText(text)


class mqttObject(QtCore.QObject):
    newData = QtCore.pyqtSignal(dict)

    def __init__(self, parent):
        super().__init__(parent)
        self.client = mqtt.Client()
        self.parent = parent
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def on_connect(self, client, userdata, flags, rc):
        self.client.subscribe("temp/#")
        self.newData.connect(self.parent.updateData)

    def on_message(self, client, userdata, msg):
        keyChain = msg.topic.split('/')
        value = str(msg.payload.decode())
        _dict = {}
        misc.setInDict(_dict, keyChain, value)
        self.newData.emit(_dict)

    def connect(self, ip=config.MQTT_SERVER_IP, port=1883):
        self.client.connect(ip, port=port, keepalive=60)
        self.client.loop_start()


if __name__ == "__main__":

    # init pyqt app
    app = QtWidgets.QApplication(sys.argv)
    treeWidget = ViewTree()
    modesWidget = ModeWidget()

    modesList = modes.loadModes()
    modeListItems = []
    for mode in modesList:
        _mode = ModeItem()
        _mode.setModeIdLabel(mode.definition['id'])
        _mode.setModeName(mode.definition["name"])
        _mode.setModeStatus(f"Active : {mode.status['active']} / Tempo : {mode.status['waiting']}")
        _modeItem = QtWidgets.QListWidgetItem()
        modesWidget.addItem(_modeItem)
        modesWidget.setItemWidget(_modeItem, _mode)
        _modeItem.setSizeHint(_mode.sizeHint())
        modeListItems.append(_mode)


    # store modes and make them accessible (states)

    # alternative
    mainWindow = QtWidgets.QMainWindow()
    mainWindow.setWindowTitle("Coop Heating")
    mainLayout = QtWidgets.QHBoxLayout()
    mainLayout.addWidget(treeWidget)
    mainLayout.addWidget(modesWidget)
    # add other widgets here to the main layout
    mainWidget = QtWidgets.QWidget()
    mainWindow.setCentralWidget(mainWidget)
    mainWidget.setLayout(mainLayout)
    mainWindow.show()

    # init mqtt
    mqttSocket = mqttObject(treeWidget)
    mqttSocket.connect()

    sys.exit(app.exec_())
