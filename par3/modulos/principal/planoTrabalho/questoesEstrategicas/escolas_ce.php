<?php
$controllerQuestoes = new Par3_Controller_QuestoEsestrategicasEscolasCe();

$qrpid = (int) $_REQUEST['qrpid'];
$perid = (int) $_REQUEST['perid'];
$inuid = (int) $_REQUEST['inuid'];

if ($qrpid && $inuid) {
    global $db;
    $strSQL = $controllerQuestoes->listaEscolasCe($_REQUEST, true);
    $rs = $db->carregar($strSQL);
    $dataSource = simec_json_encode(['data' => $rs]);
    $selecionadas = simec_json_encode($controllerQuestoes->listaSelecionadas($_REQUEST));
}

?>
<form action="par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A"
      method="POST"
      name="formularioEscolas"
      id="formularioEscolas"
      class="form form-horizontal">

    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid;?>"/>
    <input type="hidden" name="qrpid" id="qrpid" value="<?php echo $qrpid;?>"/>
    <input type="hidden" name="perid" id="perid" value="<?php echo $perid;?>"/>
    <input type="hidden" name="req" value="salvarEscolaCE"/>
    <div class="ibox">
		<div class="ibox-content">
            <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem dataTable no-footer"
                   id="formularioEscolas">
            </table>
            <input class="btn btn-success" value="Salvar" type="button" id="btn-save">
		</div>
    </div>
</form>
<script>
$(document).ready(function() {
    var dataSet=<?=$dataSource?>
      , formatDataSource=[]
      , dataSelected=<?=$selecionadas?>
    ;

    for (var i = 0; i < dataSet.data.length; i++) {
        var tmp = [
            dataSet.data[i].id,
            dataSet.data[i].co_entidade,
            dataSet.data[i].descricaomunicipio,
            dataSet.data[i].codigoescola,
            dataSet.data[i].nomeescola
        ];
        formatDataSource.push(tmp);
    }

	if (!$.fn.DataTable.isDataTable('#formularioEscolas .tabela-listagem')) {
		var table = $('#formularioEscolas .tabela-listagem').DataTable({
            'data': formatDataSource,
            'columns': [
                { title: "<input type='checkbox' class='js-switch-txt superCheck ' />", "width": "5%" },
                { title: "Entidade" },
                { title: "Municipio" },
                { title: "Código Escola" },
                { title: "Nome Escola" }
            ],
			'bFilter': true,
			'bInfo': false,
			'bLengthChange': false,
			'aoColumnDefs' : [{
				'bSortable' : false,
				'aTargets' : [ 'unsorted' ]
			},{
                'targets': 0,
                'searchable': false,
                'orderable': false,
                'className': 'dt-body-center',
                'render': function (data, type, full, meta) {
                    var co_entidade = $('<div/>').text(data).html()
                      , attrChecked = '';

                    dataSelected.forEach(function(ele, i){
                        if (ele == co_entidade) {
                            attrChecked = ' checked="checked" ';
                            return;
                        }
                    });

                    return '<input type="checkbox" '+ attrChecked +' class="js-switch-txt ck-child " co_entidade="' + co_entidade + '" qrpid="'+$("#qrpid").val()+'" perid="'+$("#perid").val()+'" name="id[]" value="' + co_entidade + '">';
                }
            },{
                "targets": [ 1 ],
                "visible": false,
                "searchable": false
            }],
			'oLanguage' : {
				'sProcessing' : "Processando...",
				'sLengthMenu' : "Mostrar _MENU_ registros",
				'sZeroRecords' : "N&atilde;o foram encontrados resultados",
				'sInfo' : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
				'sInfoEmpty' : "Mostrando de 0 at&eacute; 0 de 0 registros",
				'sInfoFiltered' : "(filtrado de _MAX_ registros no total)",
				'sInfoPostFix' : ".",
				'sSearch' : "Pesquisar :&nbsp;&nbsp;",
				'sUrl' : "",
				'oPaginate' : {
					'sFirst' : "Primeiro",
					'sPrevious' : "Anterior",
					'sNext' : "Seguinte",
					'sLast' : "&Uacute;ltimo"
				}
			}
		});
	}

    $('.superCheck').on('click', function(){
        var rows = table.rows({ 'search': 'applied' }).nodes();
        $('.ck-child', rows).prop('checked', this.checked);
    });

    $("#btn-save").on("click", function(){
        if (!$(".ck-child").is(":checked")) {
            alert("Voce precisa selecionar ao menos uma opção de Escola");
            return false;
        }

        var form = $("#formularioEscolas");
        table.$('input[type="checkbox"]').each(function(){
            if(!$.contains(document, this)){
                if(this.checked){
                    $(form).append(
                        $('<input>')
                            .attr('type', 'hidden')
                            .attr('name', this.name)
                            .val(this.value)
                    );
                }
            }
        });

        form.submit();
    });
});
</script>