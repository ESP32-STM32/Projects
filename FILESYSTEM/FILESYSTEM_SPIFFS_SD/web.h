const char Web_page[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <link rel="shortcut icon" href="favicon.png" type="image/x-icon">
    <title>Файловый менеджер</title>
    <!-- <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined" /> -->
</head>

<body>
    <style>
        body {
            display: flex;
            justify-content: center;
        }

        .container {
            width: 400px;
            height: auto;
            box-shadow: 0px 0px 40px #3b4456;
            display: flex;
            flex-direction: column;
            align-items: center;
            /* justify-content: space-around; */
            border-radius: 15px;
            font-family: 'poppins', sans-serif;
            padding: 5px;
            /* background-color: hsla(0, 0%, 0%, .1);
            backdrop-filter: blur(24px); */
        }

        .progressBar {
            background: rgb(199, 193, 193);
            width: 100%;
            height: 10px;
            margin-top: 5px;
            border-radius: 5px;
            overflow: hidden;
        }

        #progressBar {
            height: 25px;
            border-radius: 10px;
        }

        .fill {
            background-color: #ff8800;
            height: 100%;
            width: 0%;
        }

        #line1 {
            background-color: #ff8800;
            height: 100%;
            width: 0%;
        }

        .upload-form {
            width: 100%;
            height: 90px;
        }

        .form-upload__input {
            width: 100%;
            margin-top: 5px;
        }

        @keyframes fadeinout {
            0% {
                opacity: 1;
            }

            100% {
                opacity: 0;
            }
        }

        .form-upload__input::file-selector-button {
            font-size: 15px;
            min-width: 50%;
            margin-right: 10px;
            height: 25px;
            border: none;
            border-radius: 10px;
            background-color: #ff8800;
            cursor: pointer;
            transition: background-color 0.2s linear;
        }

        .form-upload__input::file-selector-button:hover {
            background-color: #00ff55;
            cursor: pointer;
        }

        .form-upload__submit {
            font-size: 15px;
            width: 100%;
            margin-top: 5px;
            height: 25px;
            border-radius: 10px;
            border: none;
            color: #000000;
            transition: background-color 0.2s linear;
            background-color: #ff8800;
        }

        .form-upload__submit:hover {
            background-color: #00ff55;
            cursor: pointer;
        }

        .form-upload__status {
            top: 0;
            padding-left: 5px;
            height: 25px;
            display: flex;
            align-items: center;
            position: absolute;
            user-select: none;
        }

        .radio {
            border-radius: 10px;
            border: 2px solid #ff8800;
            height: 25px;
            display: flex;
            position: relative;
            overflow: hidden;
            user-select: none;
        }

        .radio label {
            height: 25px;
            min-width: 70px;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
        }

        .radio input:nth-child(1):checked~span {
            transform: translateX(0%);
        }

        .radio input:nth-child(3):checked~span {
            transform: translateX(100%);
        }

        .radio span {
            position: absolute;
            background-color: #ff8800;
            width: 70px;
            height: 29px;
            color: white;
            z-index: -1;
            transition: 0.1s;
        }

        .containerFiles {
            width: 100%;
            height: auto;
            display: block;
        }

        .stroc {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .fileName {
            margin-top: 5px;
            margin-right: 5px;
            overflow: hidden;
            height: 25px;
            display: flex;
            align-items: center;
            justify-content: left;
            border-left: 6px solid #ff8800;
            border-right: 6px solid #ff8800;
            border-radius: 10px;
            width: 100%;
            background-color: #ff8800;
        }

        .fill1 {
            visibility: hidden;
            margin-top: 0px;
            position: absolute;
            width: 100%;
            height: 100%;
            background-color: rgba(255, 255, 255, .30);
        }

        .progress {
            /* display: none; */
            z-index: 1;
            margin-top: 5px;
            position: absolute;
            width: 100%;
            height: 25px;
            border-radius: 10px;
            background-image: linear-gradient(-45deg,
                    #d30505 0%,
                    #d30505 25%,
                    transparent 25%,
                    transparent 50%,
                    #d30505 50%,
                    #d30505 75%,
                    transparent 75%,
                    transparent 100%);
            background-size: 25px;
            animation: progress 1s linear infinite;
        }

        @keyframes progress {
            to {
                background-position: 25px;
            }
        }

        .btn {
            margin-top: 5px;
            height: 25px;
            background-color: #ff9100;
            border: none;
            border-radius: 10px;
            cursor: pointer;
        }
    </style>

    <div class="container" id="container">
        <div class="radio">
            <input type="radio" id="radio" style="display: none;" name="radio" onclick="fanc()">
            <label for="radio">SD</label>
            <input type="radio" id="radio1" style="display: none;" name="radio" onclick="fanc()">
            <label for="radio1">SPIFFS</label>
            <span></span>
        </div>
        <div style="margin-top: 5px;" id="used">Объем памяти</div>
        <div class="progressBar">
            <div class="fill" id="line"></div>
        </div>
        <div class="upload-form">
            <form id="uploadForm" method="post" enctype="multipart/form-data">
                <input class="form-upload__input" type="file" id="uploadForm_File">
                <input class="form-upload__submit" type="submit" value="Загрузить файл">
                <div style="position: relative; z-index: 1;">
                    <div class="progressBar" id="progressBar">
                        <div class="fill" id="line1"></div>
                    </div>
                    <div class="form-upload__status" id="upload_data">
                        <span id="uploadForm_Status">Статус</span>
                        <span id="uploadForm_Size"></span>
                    </div>
                </div>

            </form>
        </div>
    </div>
    <div class="fill1" id="fill"></div>
    <script>
        // document.getElementById('file').onchange = function () {
        //     this.setAttribute('style', 'animation: fadeinout 0.5s linear;')
        // };
        //testloadValues();
        var typefilesys = "SD";
        getSize("SD", true);
        // loadValues();
        function loadValues() {
            if (document.getElementById('err')) {
                var ttt = document.getElementById('err')
                ttt.remove()
            }
            const cont = document.getElementById('container');
            const test = document.createElement('div');
            test.className = "containerFiles"
            test.id = "err"
            cont.append(test);
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    var i = 0;
                    var myObj = JSON.parse(this.responseText);
                    while (i < myObj.length) {
                        if (myObj[i].type == "file") {
                            let div = document.createElement('div');
                            div.id = myObj[i].name
                            div.setAttribute('style', 'position: relative;')
                            div.className = "stroc"
                            div.innerHTML = '<span class="fileName">' + myObj[i].name + '</span><button class="btn" data-name="' + myObj[i].name + '">Удалить</button>';
                            test.append(div);
                        }
                        i++;
                    }
                }
            };
            xhttp.open("POST", "getFiles", true);
            xhttp.send();

            const removeHandler = event => {
                if (!event.target.dataset.name) {
                    return
                }
                const { name } = event.target.dataset
                // --------------------------------------------
                let fill = document.getElementById('fill')
                fill.setAttribute('style', 'visibility: visible; cursor: not-allowed;')
                let cont = document.getElementById(name)
                let prog = document.createElement('div')
                prog.id = "prog"
                prog.className = "progress"
                cont.append(prog)
                // --------------------------------------------
                httpDelete(name)
            }
            test.addEventListener('click', removeHandler)
        };
        function testloadValues() {
            const cont = document.getElementById('container');
            const test = document.createElement('div');
            test.className = "containerFiles"
            test.id = "err1"
            cont.append(test);
            // document.body.append(test);
            var i = 0;
            const json = '[{"type":"file","name":"edit423423423423423423424312312312312433234234234234234.htm.gz"},{"type":"file","name":"favicon.ico"},{"type":"file","name":"graphs.js.gz"},{"type":"file","name":"index.htm"}]';
            var myObj = JSON.parse(json);
            while (i < myObj.length) {
                if (myObj[i].type == "file") {
                    let div = document.createElement('div');
                    div.id = myObj[i].name
                    div.setAttribute('style', 'position: relative;')
                    div.className = "stroc"
                    div.innerHTML = '<span class="fileName">' + myObj[i].name + '</span><button class="btn" data-name="' + myObj[i].name + '">Удалить</button>';
                    test.append(div);
                }
                i++;
            }
            const removeHandler = event => {
                if (!event.target.dataset.name) {
                    return
                }
                const { name } = event.target.dataset
                // --------------------------------------------
                let fill = document.getElementById('fill')
                fill.setAttribute('style', 'visibility: visible; cursor: not-allowed;')
                let cont = document.getElementById(name)
                let prog = document.createElement('div')
                prog.id = "prog"
                prog.className = "progress"
                cont.append(prog)
                // --------------------------------------------
                httpDelete(name)
            }
            test.addEventListener('click', removeHandler)
        };

        var toggle = false;
        function fanc() {
            if (toggle && radio.checked) {
                getSize("SD", true)
                // loadValues();
                toggle = false;
                typefilesys = "SD";
            } else if (!toggle && radio1.checked) {
                getSize("SPIFFS", true)
                // loadValues();
                typefilesys = "SPIFFS";
                toggle = true;
            }
        }
        function getSize(filesystem, choice) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    var myObj = JSON.parse(this.responseText);
                    document.getElementById("used").innerHTML = myObj.used;
                    document.getElementById('line').style.width = myObj.total.toString() + '%';
                    if (choice) {
                        loadValues();
                    }
                }
            };
            var formData = new FormData();
            formData.append("path", filesystem);
            xhttp.open("POST", "getSize", true);
            xhttp.send(formData);
        }

        const BYTES_IN_MB = 1048576

        const form = document.getElementById('uploadForm')
        const fileInput = document.getElementById('uploadForm_File')
        const sizeText = document.getElementById('uploadForm_Size')
        const statusText = document.getElementById('uploadForm_Status')
        const progressBar = document.getElementById('line1')

        fileInput.addEventListener('change', function () {
            const file = this.files[0]
            if (file.size > 10 * BYTES_IN_MB) {
                alert('Принимается файл до 10 МБ')
                this.value = null
            }
        });

        form.addEventListener('submit', function (event) {
            event.preventDefault()
            const fileToUpload = fileInput.files[0]
            const formSent = new FormData()
            const xhr = new XMLHttpRequest()

            if (fileInput.files.length > 0) {
                let fill = document.getElementById('fill')
                fill.setAttribute('style', 'visibility: visible; cursor: not-allowed;')
                formSent.append('file', fileToUpload)
                // собираем запрос и подписываемся на событие progress
                xhr.upload.addEventListener('progress', progressHandler, false)
                xhr.addEventListener('load', loadHandler, false)
                xhr.open('POST', '/')
                xhr.send(formSent)
            } else {
                alert('Сначала выберите файл')
            }
            return false
        });

        function progressHandler(event) {
            // считаем размер загруженного и процент от полного размера
            const loadedMb = (event.loaded / BYTES_IN_MB).toFixed(1)
            const totalSizeMb = (event.total / BYTES_IN_MB).toFixed(1)
            const percentLoaded = Math.round((event.loaded / event.total) * 100)

            progressBar.style.width = percentLoaded + '%'
            sizeText.textContent = ` ${loadedMb} МБ из ${totalSizeMb} МБ`
            statusText.textContent = `Загружено ${percentLoaded}% | `
        }

        function loadHandler(event) {
            sizeText.textContent = ``
            statusText.textContent = `Загрузка завершена`
            //statusText.textContent = event.target.responseText
            progressBar.style.width = '0%'
            let fill = document.getElementById('fill')
            fill.setAttribute('style', 'visibility: hidden; cursor: default;')
            document.getElementById('uploadForm_File').value = '';
            getSize(typefilesys, true);
            // loadValues();
        }

        function httpDelete(filename) {
            xmlHttp = new XMLHttpRequest();
            xmlHttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    const cont = document.getElementById(filename)
                    cont.remove()
                    let fill = document.getElementById('fill')
                    fill.setAttribute('style', 'visibility: hidden; cursor: default;')
                    getSize(typefilesys, false);
                    // loadValues();
                }
            };
            var formData = new FormData();
            formData.append("path", filename);
            xmlHttp.open("DELETE", "/");
            xmlHttp.send(formData);
        }
    </script>

</body>

</html>
)=====";