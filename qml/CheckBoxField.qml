import QtQuick 2.0
import QtQuick 2.4
import QtQuick.Controls 1.3

Rectangle {
    id:root
    property string text
    property color textColor: "black"
    property alias availableValues: selectvalues.model
    property int hAlign: TextInput.AlignLeft
    property int vAlign: TextInput.AlignTop
    property bool clippedText: false
    signal textHasChanged(string value)
    TextField {
        id: textField
        anchors.fill: parent
        selectByMouse: true
        onTextChanged: {
           computeSizeFont();
           console.log("Changed text:"+text)
           root.textHasChanged(text)
        }
    }
    TextInput {//textInput.textColor
        id: textInput
        anchors.fill: parent
        selectByMouse: true
        horizontalAlignment: root.hAlign
        verticalAlignment: root.vAlign
        onTextChanged: {
           computeSizeFont();
            console.log("Changed text:"+text)
            root.textHasChanged(text)
        }
        onWidthChanged: {
            computeSizeFont();
        }
        function computeSizeFont()
        {
            if(clippedText)
            {
                while((contentWidth>root.width)&&(font.pointSize>1)&&(root.width>0))
                {
                    font.pointSize-=1
                }
                while((contentWidth+2<width)&&(contentHeight+2<height))
                {
                    font.pointSize+=1
                }
            }
        }
    }
    TextArea {
        id: textArea
        anchors.fill: parent
        selectByMouse: true
        onTextChanged: {
           root.textHasChanged(text)
        }
    }
    ComboBox {
        id: selectvalues
        anchors.fill: parent
        //onCurrentTextChanged: root.text = selectvalues.currentText
        onCurrentTextChanged: {
           root.textHasChanged(text)
        }
    }
    CheckBox {
        id: checkbox
        anchors.fill: parent
        //onCheckedChanged:
        onCheckedChanged: {
           root.textHasChanged(checked == true ? root.text = "X" : root.text = "")
        }
    }

    states: [
    State {
            name: ""
            PropertyChanges {target: selectvalues; visible: false}
            PropertyChanges {target: textInput; visible: false}
            PropertyChanges {target: textField; text: root.text}
            PropertyChanges {target: selectvalues; currentIndex: find(root.text)}
            PropertyChanges {target: textArea; visible: false}
            PropertyChanges {target: textField; visible: false}
            PropertyChanges {target: checkbox; visible: false}
        },
    State {
            name: "field"
            PropertyChanges {target: selectvalues; visible: false}
            PropertyChanges {target: textField; visible: true}
            PropertyChanges {target: textField; text: root.text}
            PropertyChanges {target: textField; textColor: root.textColor}
            PropertyChanges {target: textArea; visible: false}
            PropertyChanges {target: checkbox; visible: false}
        },
        State {
                name: "input"
                PropertyChanges {target: selectvalues; visible: false}
                PropertyChanges {target: textInput; visible: true}
                PropertyChanges {target: textField; visible: false}
                PropertyChanges {target: textArea; visible: false}
                PropertyChanges {target: checkbox; visible: false}
            },
        State {
            name: "combo"
            PropertyChanges {target: textInput; visible: false}
            PropertyChanges {target: selectvalues; visible: true}          
            PropertyChanges {target: selectvalues; currentIndex: find(root.text)}
            PropertyChanges {target: textArea; visible: false}
            PropertyChanges {target: checkbox; visible: false}
            PropertyChanges {target: textField; visible: false}
        },
        State {
            name: "textarea"
            PropertyChanges {target: textInput; visible: false}
            PropertyChanges {target: selectvalues; visible: false}
            PropertyChanges {target: textArea; visible: true}
            PropertyChanges {target: textArea; textColor: root.textColor}
            PropertyChanges {target: textArea; text: root.text}
            PropertyChanges {target: checkbox; visible: false}
            PropertyChanges {target: textField; visible: false}
        },
        State {
            name: "check"
            PropertyChanges {target: textInput; visible: false}
            PropertyChanges {target: selectvalues; visible: false}
            PropertyChanges {target: textArea; visible: false}
            PropertyChanges {target: checkbox; visible: true}
            PropertyChanges {target: textField; visible: false}
            PropertyChanges {target: checkbox; checked: root.text == "X" ? true : false}
        }



    ]
}

