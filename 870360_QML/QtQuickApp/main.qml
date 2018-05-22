import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.2

// "1. Реализовать показанный на рисунке графический интерфейс ..."

Window {
    id: mainWnd
    visible: true
    width: 280
    height: 160
    title: qsTr("Пребразователь температур")
    // минимальные размеры окна, при которых
    // интерфейс остаётся пригоден к использованию
    minimumWidth: 200
    minimumHeight: 130
    // значение температуры изменено вручную,
    // а не пересчитано автоматически
    property bool manualChange: true

    Label {
        id: lblFar
        x: 10
        y: 10
        text: qsTr("Градусы Фаренгейта:")
    }

    TextEdit {
        id: txtFar
        x: 140
        y: 10
        width: parent.width - 150
        verticalAlignment: Text.AlignVCenter
        text: "97.88" // Math.round(txtCel.text * 1.800 + 32) / 100
        Rectangle {
            anchors.fill: parent
            anchors.margins: -4
            color: "transparent"
            border.width: 1
        }
        // "2. Можно ли сделать интерфейс более эффективным? ..."
        // При связывании со свойством-источником,
        // пользователю не требуется нажимать кнопку для вычислений.
        // Однако, во избежание зацикливания, используем вместо
        // связанных свойств события onTextChanged и логический
        // флаг ручного/автоматического изменения текста manualChange
        onTextChanged: {
            if(mainWnd.manualChange) {
                mainWnd.manualChange = false
                txtCel.text = Math.round((Number(text) - 32) * 100 / 1.8) / 100
                txtKel.text = Number(txtCel.text) + 273.15
                //console.log("F(" + text + ") -> C(" + txtCel.text + ") K(" + txtKel.text + ")")
                mainWnd.manualChange = true
            }
        }
    }

    Label {
        id: lblCel
        x: 10
        y: 40
        text: qsTr("Градусы Цельсия:")
    }

    TextEdit {
        id: txtCel
        x: 140
        y: 40
        width: txtFar.width
        verticalAlignment: Text.AlignVCenter
        Rectangle {
            anchors.fill: parent
            anchors.margins: -4
            color: "transparent"
            border.width: 1
        }
        // было: text: "0.00" // Math.round((txtFar.text - 32) * 100 / 1.8) / 100
        onTextChanged: {
            // "3. Добавьте возможность преобразования в обратную сторону ..."
            if(mainWnd.manualChange) {
                mainWnd.manualChange = false
                txtFar.text = Math.round((Number(text) * 1.8 + 32) * 100) / 100
                txtKel.text = Number(text) + 273.15
                mainWnd.manualChange = true
                //console.log("C(" + text + ") -> F(" + txtFar.text + ") K(" + txtKel.text + ")")
            }
        }
    }

    Label {
        id: lblKel
        x: 10
        y: 70
        text: qsTr("Градусы Кельвина:")
    }

    // "4. Добавьте также возможность использовать температуру в Кельвинах
    // с возможностью преобразования между любыми из трёх единиц измерения"
    TextEdit {
        id: txtKel
        x: 140
        y: 70
        width: txtFar.width
        verticalAlignment: Text.AlignVCenter
        //text: "0.00"
        Rectangle {
            anchors.fill: parent
            anchors.margins: -4
            color: "transparent"
            border.width: 1
        }
        // Из Кельвина в Цельсия и Фаренгейта преобразуем точно так же
        onTextChanged: {
            if(mainWnd.manualChange) {
                mainWnd.manualChange = false
                txtCel.text = Math.round((Number(text) - 273.15) * 100) / 100
                txtFar.text = Math.round((Number(txtCel.text) * 1.8 + 32) * 100) / 100
                //console.log("K(" + text + ") -> C(" + txtCel.text + ") F(" + txtFar.text + ")")
                mainWnd.manualChange = true
            }
        }
    }

    Button {
        x: parent.width - width - 10;
        y: parent.height - height - 10;
        text: "Преобразовать"
        onClicked: {
            // Поскольку кнопку освободили в п.2, сделав пересчёт автоматическим,
            // она больше не нужна и можно её удалить с формы
            // было: txtCel.text = Math.round((txtFar.text - 32) * 100 / 1.8) / 100
        }
    }
}
