<script>
    var dadosForm = $("#formulario").serializeArray();
    var  tipoProcessoParObra = dadosForm.findIndex(val => val.name == 'tipo');
    var origemFormulario = $("#formulario").serialize();
    var resultado = origemFormulario.replace(/[^&]+=\.?(?:&|$)/g, '')
    if (dadosForm.findIndex(val => val.name == 'analise') == -1){
        $("#divAnalise<?php echo $ArrPro['proid'];?>").hide();
    } else {
        if(dadosForm[tipoProcessoParObra].value == 'O'){
            $("#analise<?php echo $ArrPro['proid'];?>").jqGrid({
                url: window.location.href + "&requisicao=" + 'listarAnalise&proid=' + <?php echo $ArrPro['proid'];?>+ '&' + resultado,
                editurl: window.location.href + "&requisicao=" + 'editarAnaliseObra' + "&" + $("#formulario").serialize(),
                mtype: "GET",
                datatype: "json",
                page: 1,
                caption: "Análise",
                colModel: [
                    {
                        label: "#",
                        name: "actions",
                        width: 100,
                        formatter: "actions",
                        formatoptions: {
                            keys: true,
                            editOptions: {},
                        }
                    },
                    {
                        label: 'obrid',
                        name: 'obrid',
                        key: true,
                        width: 200,
                    },
                    {
                        label: 'otpid',
                        name: 'otpid',
                        editable: true
                    },
                    {
                        label: 'ovpid',
                        name: 'ovpid',
                        editable: true
                    },
                    {
                        label: 'tplid',
                        name: 'tplid',
                        editable: true
                    },
                    {
                        label: 'inuid',
                        name: 'inuid',
                        editable: true
                    },
                    {
                        label: 'muncod',
                        name: 'muncod',
                        editable: true
                    },
                    {
                        label: 'obrano',
                        name: 'obrano',
                        editable: true
                    },
                    {
                        label: 'obrstatus',
                        name: 'obrstatus',
                        editable: true
                    },

                ],
                loadonce: true,
                onSelectRow: editRow,
                viewrecords: true,
                width: 1100,
                height:'auto'
            });
        }else{
            $("#analise<?php echo $ArrPro['proid'];?>").jqGrid({
                url: window.location.href + "&requisicao=" + 'listarAnalise&proid=' + <?php echo $ArrPro['proid'];?>+ '&' + resultado,
                editurl: window.location.href + "&requisicao=" + 'editarAnalisePar' + "&" + $("#formulario").serialize(),
                mtype: "GET",
                datatype: "json",
                page: 1,
                caption: "Análise",
                colModel: [
                    {
                        label: "#",
                        name: "actions",
                        width: 100,
                        formatter: "actions",
                        formatoptions: {
                            keys: true,
                            editOptions: {},
                        }
                    },
                    {
                        label: 'anaid',
                        name: 'anaid',
                        key: true,
                        width: 200,
                    },
                    {
                        label: 'inpid',
                        name: 'inpid',
                        editable: true
                    },
                    {
                        label: 'anaano',
                        name: 'anaano',
                        editable: true
                    },
                    {
                        label: 'anaparecer',
                        name: 'anaparecer',
                        editable: true
                    },
                    {
                        label: 'anastatus',
                        name: 'anastatus',
                        editable: true
                    },
                    {
                        label: 'intaid',
                        name: 'intaid',
                        editable: true
                    },
                    {
                        label: 'edeid',
                        name: 'edeid',
                        editable: true
                    },

                ],
                loadonce: true,
                onSelectRow: editRow,
                viewrecords: true,
                width: 1100,
                height:'auto'
            });
        }
    }
    if (dadosForm.findIndex(val => val.name == 'documento') == -1){
        $("#divDocumento<?php echo $ArrPro['proid'];?>").hide();
    } else {
        $("#documento<?php echo $ArrPro['proid'];?>").jqGrid({
            url: window.location.href + "&requisicao=" + 'listarDocumento&proid=' + <?php echo $ArrPro['proid'];?>,
            editurl: window.location.href + "&requisicao=" + 'editarDocumento' + "&" + $("#formulario").serialize(),
            mtype: "GET",
            datatype: "json",
            page: 1,
            caption: "Documento",
            colModel: [
                {
                    label: "#",
                    name: "actions",
                    width: 100,
                    formatter: "actions",
                    formatoptions: {
                        keys: true,
                        editOptions: {},
                    }
                },
                {
                    label: 'dotid',
                    name: 'dotid',
                    key: true,
                    width: 200,
                },
                {
                    label: 'proid',
                    name: 'proid',
                    editable: true
                },
                {
                    label: 'dotnumero',
                    name: 'dotnumero',
                    editable: true
                },
                {
                    label: 'dotstatus',
                    name: 'dotstatus',
                    editable: true
                },
                {
                    label: 'dotdatainclusao',
                    name: 'dotdatainclusao',
                    editable: true
                },                {
                    label: 'dotdataexclusao',
                    name: 'dotdataexclusao',
                    editable: true
                },                {
                    label: 'dotusucpfexcluxao',
                    name: 'dotusucpfexcluxao',
                    editable: true
                },

            ],
            loadonce: true,
            onSelectRow: editRow,
            viewrecords: true,
            width: 1100  ,
            height:'auto'
        });
    }
    if (dadosForm.findIndex(val => val.name == 'empenho') == -1){
        $("#divEmpenho<?php echo $ArrPro['proid'];?>").hide();
    } else {
        $("#empenho<?php echo $ArrPro['proid'];?>").jqGrid({
            url: window.location.href + "&requisicao=" + 'listarEmpenho&proid=' + <?php echo $ArrPro['proid'];?>,
            editurl: window.location.href + "&requisicao=" + 'editarEmpenho' + "&" + $("#formulario").serialize(),
            mtype: "GET",
            datatype: "json",
            page: 1,
            caption: "Empenho",
            colModel: [
                {
                    label: "#",
                    name: "actions",
                    width: 100,
                    formatter: "actions",
                    formatoptions: {
                        keys: true,
                        editOptions: {},
                    }
                },
                {
                    label: 'empid',
                    name: 'empid',
                    key: true,
                    width: 200,
                },
                {
                    label: 'empid',
                    name: 'empid',
                    editable: true
                },
                {
                    label: 'empcnpj',
                    name: 'empcnpj',
                    editable: true
                },
                {
                    label: 'empidpai',
                    name: 'empidpai',
                    editable: true
                },
                {
                    label: 'empnumero',
                    name: 'empnumero',
                    editable: true
                },
                {
                    label: 'empvalorempenho',
                    name: 'empvalorempenho',
                    editable: true
                },
                {
                    label: 'empstatus',
                    name: 'empstatus',
                    editable: true
                },
                {
                    label: 'empcodigoespecie',
                    name: 'empcodigoespecie',
                    editable: true
                },
                {
                    label: 'empsituacao',
                    name: 'empsituacao',
                    editable: true
                },

            ],
            loadonce: true,
            onSelectRow: editRow,
            viewrecords: true,
            width: 1100  ,
            height:'auto'
        });
        if(dadosForm[tipoProcessoParObra].value == 'O'){
            $("#empenhocomposicao<?php echo $ArrPro['proid'];?>").jqGrid({
                url: window.location.href + "&requisicao=" + 'listarEmpenhoComposicao&proid=' + <?php echo $ArrPro['proid'];?>+ '&' + resultado,
                editurl: window.location.href + "&requisicao=" + 'editarEmpenhoComposicaoObra' + "&" + $("#formulario").serialize(),
                mtype: "GET",
                datatype: "json",
                page: 1,
                caption: "Empenho Composição (Obra)",
                colModel: [
                    {
                        label: "#",
                        name: "actions",
                        width: 100,
                        formatter: "actions",
                        formatoptions: {
                            keys: true,
                            editOptions: {},
                        }
                    },
                    {
                        label: 'eocid',
                        name: 'eocid',
                        key: true,
                        width: 200,
                    },
                    {
                        label: 'empid',
                        name: 'empid',
                        editable: true
                    },
                    {
                        label: 'pocid',
                        name: 'pocid',
                        editable: true
                    },
                    {
                        label: 'eocpercentualemp',
                        name: 'eocpercentualemp',
                        editable: true
                    },
                    {
                        label: 'eocvalorempenho',
                        name: 'eocvalorempenho',
                        editable: true
                    },
                    {
                        label: 'eocstatus',
                        name: 'eocstatus',
                        editable: true
                    },
                ],
                loadonce: true,
                onSelectRow: editRow,
                viewrecords: true,
                width: 1100,
                height:'auto'
            });
        }else{
            $("#empenhocomposicao<?php echo $ArrPro['proid'];?>").jqGrid({
                url: window.location.href + "&requisicao=" + 'listarEmpenhoComposicao&proid=' + <?php echo $ArrPro['proid'];?>+ '&' + resultado,
                editurl: window.location.href + "&requisicao=" + 'editarEmpenhoComposicaoPar' + "&" + $("#formulario").serialize(),
                mtype: "GET",
                datatype: "json",
                page: 1,
                caption: "Empenho Composição (Par)",
                colModel: [
                    {
                        label: "#",
                        name: "actions",
                        width: 100,
                        formatter: "actions",
                        formatoptions: {
                            keys: true,
                            editOptions: {},
                        }
                    },
                    {
                        label: 'epcid',
                        name: 'epcid',
                        key: true,
                        width: 200,
                    },
                    {
                        label: 'empid',
                        name: 'empid',
                        editable: true
                    },
                    {
                        label: 'ppcid',
                        name: 'ppcid',
                        editable: true
                    },
                    {
                        label: 'epcpercentualemp',
                        name: 'epcpercentualemp',
                        editable: true
                    },
                    {
                        label: 'epcvalorempenho',
                        name: 'epcvalorempenho',
                        editable: true
                    },
                    {
                        label: 'epcstatus',
                        name: 'epcstatus',
                        editable: true
                    },
                ],
                loadonce: true,
                onSelectRow: editRow,
                viewrecords: true,
                width: 1100,
                height:'auto'
            });
        }
    }
    if (dadosForm.findIndex(val => val.name == 'pagamento') == -1){
        $("#divPagamento<?php echo $ArrPro['proid'];?>").hide();
        $("#divPagamentoComposicao<?php echo $ArrPro['proid'];?>").hide();
    } else {
        $("#pagamento<?php echo $ArrPro['proid'];?>").jqGrid({
            url: window.location.href + "&requisicao=" + 'listarPagamento&proid=' + <?php echo $ArrPro['proid'];?>,
            editurl: window.location.href + "&requisicao=" + 'editarPagamento' + "&" + $("#formulario").serialize(),
            mtype: "GET",
            datatype: "json",
            caption: "Pagamento",
            page: 1,
            colModel: [
                {
                    label: "#",
                    name: "actions",
                    width: 100,
                    formatter: "actions",
                    formatoptions: {
                        keys: true,
                        editOptions: {},
                    }
                },
                {
                    label: 'pagid',
                    name: 'pagid',
                    key: true,
                    width: 200,
                },
                {
                    label: 'pagparcela',
                    name: 'pagparcela',
                    editable: true
                },
                {
                    label: 'paganoexercicio',
                    name: 'paganoexercicio',
                    editable: true
                },
                {
                    label: 'empid',
                    name: 'empid',
                    editable: true
                },
                {
                    label: 'pagstatus',
                    name: 'pagstatus',
                    editable: true
                },

            ],
            loadonce: true,
            onSelectRow: editRow,
            viewrecords: true,
            width: 1100  ,
            height:'auto'
        });
        if(dadosForm[tipoProcessoParObra].value == 'O'){
            $("#pagamentocomposicao<?php echo $ArrPro['proid'];?>").jqGrid({
                url: window.location.href + "&requisicao=" + 'listarPagamentoComposicao&proid=' + <?php echo $ArrPro['proid'];?>+ '&' + resultado,
                editurl: window.location.href + "&requisicao=" + 'editarPagamentoComposicaoObra' + "&" + $("#formulario").serialize(),
                mtype: "GET",
                datatype: "json",
                page: 1,
                caption: "Pagamento Composição (Obra)",
                colModel: [
                    {
                        label: "#",
                        name: "actions",
                        width: 100,
                        formatter: "actions",
                        formatoptions: {
                            keys: true,
                            editOptions: {},
                        }
                    },
                    {
                        label: 'pmcid',
                        name: 'pmcid',
                        key: true,
                        width: 200,
                    },
                    {
                        label: 'pagid',
                        name: 'pagid',
                        editable: true
                    },
                    {
                        label: 'eocid',
                        name: 'eocid',
                        editable: true
                    },
                    {
                        label: 'pmcpercentualpag',
                        name: 'pmcpercentualpag',
                        editable: true
                    },
                    {
                        label: 'pmcvalorpagamento',
                        name: 'pmcvalorpagamento',
                        editable: true
                    },
                    {
                        label: 'pmcstatus',
                        name: 'pmcstatus',
                        editable: true
                    },


                ],
                loadonce: true,
                onSelectRow: editRow,
                viewrecords: true,
                width: 1100  ,
                height:'auto'
            });
        }else{
            $("#pagamentocomposicao<?php echo $ArrPro['proid'];?>").jqGrid({
                url: window.location.href + "&requisicao=" + 'listarPagamentoComposicao&proid=' + <?php echo $ArrPro['proid'];?>+ '&' + resultado,
                editurl: window.location.href + "&requisicao=" + 'editarPagamentoComposicaoPar' + "&" + $("#formulario").serialize(),
                mtype: "GET",
                datatype: "json",
                page: 1,
                caption: "Pagamento Composição (Par)",
                colModel: [
                    {
                        label: "#",
                        name: "actions",
                        width: 100,
                        formatter: "actions",
                        formatoptions: {
                            keys: true,
                            editOptions: {},
                        }
                    },
                    {
                        label: 'ppcid',
                        name: 'ppcid',
                        key: true,
                        width: 200,
                    },
                    {
                        label: 'pagid',
                        name: 'pagid',
                        editable: true
                    },
                    {
                        label: 'epcid',
                        name: 'epcid',
                        editable: true
                    },
                    {
                        label: 'ppcpercentualpag',
                        name: 'ppcpercentualpag',
                        editable: true
                    },
                    {
                        label: 'ppcvalorpagamento',
                        name: 'ppcvalorpagamento',
                        editable: true
                    },
                    {
                        label: 'ppcstatus',
                        name: 'ppcstatus',
                        editable: true
                    },


                ],
                loadonce: true,
                onSelectRow: editRow,
                viewrecords: true,
                width: 1100  ,
                height:'auto'
            });
        }
    }


</script>

<!-- Composição -->
<div>
    <table id="composicao<?php echo $ArrPro['proid'];?>"></table>
    <div id="jqGridPager"></div>
</div>
<br/>

<script>
    var dadosForm = $("#formulario").serializeArray();
    var tipoProcessoParObra = dadosForm.findIndex(val => val.name == 'tipo');
    var origemFormulario = $("#formulario").serialize();
    var resultado = origemFormulario.replace(/[^&]+=\.?(?:&|$)/g, '');


    if(dadosForm[tipoProcessoParObra].value == 'O'){
        $("#composicao<?php echo $ArrPro['proid'];?>").jqGrid({
            url: window.location.href + "&requisicao=" + 'listarComposicao&proid=' + <?php echo $ArrPro['proid'];?> + "&" +resultado,
            editurl: window.location.href + "&requisicao=" + 'editarComposicaoObra' + "&" + $("#formulario").serialize(),
            mtype: "GET",
            datatype: "json",
            page: 1,
            colModel: [
                {
                    label: "#",
                    name: "actions",
                    width: 100,
                    formatter: "actions",
                    formatoptions: {
                        keys: true,
                        editOptions: {},
                    }
                },
                {
                    label: 'pocid',
                    name: 'pocid',
                    key: true,
                    width: 200,
                },
                {
                    label: 'proid',
                    name: 'proid',
                    editable: true
                },
                {
                    label: 'obrid',
                    name: 'obrid',
                    editable: true,
                },
                {
                    label: 'pocstatus',
                    name: 'pocstatus',
                    editable: true,
                },
                {
                    label: 'intoid',
                    name: 'intoid',
                    editable: true,
                },
            ],
            loadonce: true,
            onSelectRow: editRow,
            viewrecords: true,
            width: 1100  ,
            height:'auto',
            caption: "Composição",
        });
    }else{
        $("#composicao<?php echo $ArrPro['proid'];?>").jqGrid({
            url: window.location.href + "&requisicao=" + 'listarComposicao&proid=' + <?php echo $ArrPro['proid'];?> + "&" +resultado,
            editurl: window.location.href + "&requisicao=" + 'editarComposicaoPar' + "&" + $("#formulario").serialize(),
            mtype: "GET",
            datatype: "json",
            page: 1,
            colModel: [
                {
                    label: "#",
                    name: "actions",
                    width: 100,
                    formatter: "actions",
                    formatoptions: {
                        keys: true,
                        editOptions: {},
                    }
                },
                {
                    label: 'ppcid',
                    name: 'ppcid',
                    key: true,
                    width: 200,
                },
                {
                    label: 'proid',
                    name: 'proid',
                    editable: true
                },
                {
                    label: 'inpid',
                    name: 'inpid',
                    editable: true,
                },
                {
                    label: 'ppcano',
                    name: 'ppcano',
                    editable: true,
                },
                {
                    label: 'ppcstatus',
                    name: 'ppcstatus',
                    editable: true,
                },
                {
                    label: 'anaid',
                    name: 'anaid',
                    editable: true,
                },
                {
                    label: 'intoid',
                    name: 'intoid',
                    editable: true,
                },


            ],
            loadonce: true,
            onSelectRow: editRow,
            viewrecords: true,
            width: 1100  ,
            height:'auto',
            caption: "Composição",
        });
    }

    var lastSelection

    function editRow(id) {
        if (id && id !== lastSelection) {
            var grid = $("#processo");
            saveEditedRow(lastSelection);
            grid.jqGrid('restoreRow', lastSelection);
            grid.jqGrid('editRow', id,
                {
                    keys: true,
                    focusField: 4,
                    errorfunc: function (data) {
                        console.log(data);
                        alert('editou');
                    }
                }
            );
            lastSelection = id;
        }
    }

    function saveEditedRow(id) {
        var grid = $("#composicao");
        grid.jqGrid('saveRow', id, 'clientArray');
    }

</script>

<!--Análise do processo-->
<div id="divAnalise<?php echo $ArrPro['proid'];?>">
    <div>
        <table id="analise<?php echo $ArrPro['proid'];?>"></table>
        <div id="jqGridPager"></div>
    </div>
    <br/>

    <script>

        var lastSelection

        function editRow(id) {
            if (id && id !== lastSelection) {
                var grid = $("#analise");
                saveEditedRow(lastSelection);
                grid.jqGrid('restoreRow', lastSelection);
                grid.jqGrid('editRow', id,
                    {
                        keys: true,
                        focusField: 4,
                        errorfunc: function (data) {
                            console.log(data);
                            alert('editou');
                        }
                    }
                );
                lastSelection = id;
            }
        }

        function saveEditedRow(id) {
            var grid = $("#analise");
            grid.jqGrid('saveRow', id, 'clientArray');
        }

    </script>
</div>

<!--Documento do processo-->
<div id="divDocumento<?php echo $ArrPro['proid'];?>">
    <div>
        <table id="documento<?php echo $ArrPro['proid'];?>"></table>
        <div id="jqGridPager"></div>
    </div>
    <br/>
    <script>
        var lastSelection

        function editRow(id) {
            if (id && id !== lastSelection) {
                var grid = $("#documento");
                saveEditedRow(lastSelection);
                grid.jqGrid('restoreRow', lastSelection);
                grid.jqGrid('editRow', id,
                    {
                        keys: true,
                        focusField: 4,
                        errorfunc: function (data) {
                            console.log(data);
                            alert('editou');
                        }
                    }
                );
                lastSelection = id;
            }
        }

        function saveEditedRow(id) {
            var grid = $("#documento");
            grid.jqGrid('saveRow', id, 'clientArray');
        }

    </script>
</div>

<!--Documento do processo-->
<div id="divEmpenho<?php echo $ArrPro['proid'];?>">
    <div>
        <table id="empenho<?php echo $ArrPro['proid'];?>"></table>
        <div id="jqGridPager"></div>
    </div>
    <br/>

    <script>

        var lastSelection

        function editRow(id) {
            if (id && id !== lastSelection) {
                var grid = $("#empenho");
                saveEditedRow(lastSelection);
                grid.jqGrid('restoreRow', lastSelection);
                grid.jqGrid('editRow', id,
                    {
                        keys: true,
                        focusField: 4,
                        errorfunc: function (data) {
                            console.log(data);
                            alert('editou');
                        }
                    }
                );
                lastSelection = id;
            }
        }

        function saveEditedRow(id) {
            var grid = $("#empenho");
            grid.jqGrid('saveRow', id, 'clientArray');
        }

    </script>
</div>
<div id="divEmpenhoComposicao<?php echo $ArrPro['proid'];?>">
    <div>
        <table id="empenhocomposicao<?php echo $ArrPro['proid'];?>"></table>
        <div id="jqGridPager"></div>
    </div>
    <br/>

    <script>

        var lastSelection

        function editRow(id) {
            if (id && id !== lastSelection) {
                var grid = $("#empenhocomposicao");
                saveEditedRow(lastSelection);
                grid.jqGrid('restoreRow', lastSelection);
                grid.jqGrid('editRow', id,
                    {
                        keys: true,
                        focusField: 4,
                        errorfunc: function (data) {
                            console.log(data);
                            alert('editou');
                        }
                    }
                );
                lastSelection = id;
            }
        }

        function saveEditedRow(id) {
            var grid = $("#empenhocomposicao");
            grid.jqGrid('saveRow', id, 'clientArray');
        }

    </script>
</div>
<!--Documento do processo-->
<div id="divPagamento<?php echo $ArrPro['proid'];?>">
    <div>
        <table id="pagamento<?php echo $ArrPro['proid'];?>"></table>
        <div id="jqGridPager"></div>
    </div>
    <br/>
    <script>

        var lastSelection

        function editRow(id) {
            if (id && id !== lastSelection) {
                var grid = $("#pagamento");
                saveEditedRow(lastSelection);
                grid.jqGrid('restoreRow', lastSelection);
                grid.jqGrid('editRow', id,
                    {
                        keys: true,
                        focusField: 4,
                        errorfunc: function (data) {
                            console.log(data);
                            alert('editou');
                        }
                    }
                );
                lastSelection = id;
            }
        }

        function saveEditedRow(id) {
            var grid = $("#pagamento");
            grid.jqGrid('saveRow', id, 'clientArray');
        }

    </script>
</div>

<!--Pagamento composição do processo-->
<div id="divPagamentoComposicao<?php echo $ArrPro['proid'];?>">
    <div>
        <table id="pagamentocomposicao<?php echo $ArrPro['proid'];?>"></table>
        <div id="jqGridPager"></div>
    </div>
    <br/>
    <script>

        var lastSelection

        function editRow(id) {
            if (id && id !== lastSelection) {
                var grid = $("#pagamentocomposicao");
                saveEditedRow(lastSelection);
                grid.jqGrid('restoreRow', lastSelection);
                grid.jqGrid('editRow', id,
                    {
                        keys: true,
                        focusField: 4,
                        errorfunc: function (data) {
                            console.log(data);
                            alert('editou');
                        }
                    }
                );
                lastSelection = id;
            }
        }

        function saveEditedRow(id) {
            var grid = $("#pagamentocomposicao");
            grid.jqGrid('saveRow', id, 'clientArray');
        }

    </script>
</div>
