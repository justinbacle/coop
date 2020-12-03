import sys
from PyQt5 import QtWidgets, QtCore
import paho.mqtt.client as mqtt


class ViewTree(QtWidgets.QTreeWidget):
    def __init__(self):
        super().__init__()

        # refresh timer
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.refresh)
        self.timer.start(1000)

        # set qtreewidget
        self.setColumnCount(2)

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
        self.dataDict.update(dataDict)
        self.refresh()


def updateNestedDict(d, key_lst, val):
    for k in key_lst[:-1]:
        if k not in d:
            d[k] = {}
        d = d[k]
    d[key_lst[-1]] = val


def initMqtt():
    def on_connect(client, userdata, flags, rc):
        client.subscribe("temp/#")

    def on_message(client, userdata, msg):
        keyChain = msg.topic.split('/')
        value = str(msg.payload)
        # How to emit to the viewTree updateData ?
        # .emit(
        #     updateNestedDict({}, keyChain, value)
        # )

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("192.168.1.100", 1883, 60)
    client.loop_forever()


if __name__ == "__main__":
    # init mqtt
    initMqtt()

    app = QtWidgets.QApplication(sys.argv)
    treeWidget = ViewTree()
    treeWidget.show()
    sys.exit(app.exec_())
