import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import Qt.labs.qmlmodels 1.0

ApplicationWindow {

    id: appwindow
    visible: true
    width: 640; height: 480
    title: qsTr("test vic")
    property var horizontal_header_data: ["номер", "id", "fio", "tel" ]

    Timer{
        id: readStarter
        interval: 0
        running: true
        repeat:  false
        onTriggered: filedialog.open();
    }

    // file dialog to pick json file
    FileDialog {
        id: filedialog
        title: "Select a txt file"
        folder: shortcuts.documents
        nameFilters: ["Txt (*.txt)"]
        onAccepted: {
            readTextFile(filedialog.fileUrls[0])
        }
    }
    function readTextFile(fileUrl){
       var xhr = new XMLHttpRequest;
       xhr.open("GET", fileUrl); // set Method and File
       xhr.onreadystatechange = function () {
           if(xhr.readyState === XMLHttpRequest.DONE){ // if request_status == DONE
               var response = xhr.responseText;

               var str_array = response.split('\n');
               for (var i = 0; i < str_array.length; i++) {

                   var param_array= str_array[i].split('|')
                   let row_data = {
                            txt1: param_array[0],
                            txt2: param_array[1],
                            txt3: param_array[2],
                            txt4: param_array[3]
                            }
                   tablemodel.appendRow(row_data)
                 }
               tableview.forceLayout()
           }
       }
       xhr.send(); // begin the request
   }

    Column {

        height: parent.height; width: parent.width

       TableView {
            id: tableview
            width: parent.width; height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter


            leftMargin: 0
            topMargin: horizontalHeader.height

            ScrollIndicator.horizontal: ScrollIndicator { }
            ScrollIndicator.vertical: ScrollIndicator { }

           columnWidthProvider: function(column){ return tableview.width/4 }
           rowHeightProvider: function (column) { return 40 }

//           Обновить таблицу при изменении размера
            onWidthChanged: {
                tableview.forceLayout();
            }

            // Заголовок
            Row {
                id: horizontalHeader
                y: tableview.contentY
                z: 2
                Repeater {
                    model: tableview.columns
                    Label {
                        width: tableview.columnWidthProvider(modelData)
                        height: tableview.rowHeightProvider(modelData)
                        text: horizontal_header_data[index]
                        padding: 10
                        verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                        color: "white"
                        background: Rectangle { color: "gray" }
                    }
                }
            }

            model: TableModel {
                id: tablemodel
                TableModelColumn{ display: "txt1" }
                TableModelColumn{ display: "txt2" }
                TableModelColumn{ display: "txt3" }
                TableModelColumn{ display: "txt4" }
            }
//            Тело таблицы
            delegate:
                TextField {
                    width: tableview.columnWidthProvider(modelData)
                    height: tableview.rowHeightProvider(modelData)
                    text: display
                    padding: 10

                }

        }

    }

}
