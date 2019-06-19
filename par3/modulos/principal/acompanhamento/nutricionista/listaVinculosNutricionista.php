
<style>
.detalhe{
    cursor: pointer;
}
.detalhe:hover{
    color: #bfbfbf;
}
</style>
<script>
$(document).ready(function(){

	/* Necessário poruqe esta tela pode ser carregada diversas vezes e antes a função era carregada junto com a tela e quando solicitada, fazia várias requisições Ajax */
	if (typeof varCarregado == 'undefined') {
		varCarregado = '';
	}
	
	if( varCarregado < 1) 
	{
	    $('.detalhe').click(function(){

	    	varCarregado++;
	    	var danid = $(this).attr('danid');
	    	var tipo_id = $(this).attr('tipo_id');
	
	    	$.ajax({
	            type: 'post',
	            url: window.location.href,
	            data: 'req=visualisarNutricionista&danid='+danid+'&tipo_id='+tipo_id,
	            async: false,
	            success: function (res) {
	            	$('#modal_detalhe').html(res);
	            }
	        });
	    });
    }
});
</script>
<div class="ibox-content">


    <?php

  if($_REQUEST['xls'] == 1) {

      header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
      header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
      header("Cache-Control: no-cache, must-revalidate");
      header("Pragma: no-cache");
      header("Content-type: application/x-msexcel");
      header("Content-Disposition: attachment; filename=Relatorio_Nutricionistas".time().".xls");
      header("Content-Description: PHP Generated Data");
  }
//global $simec;
?>




    <table class="table table-hover" style="border: 1px solid #999" id="listaVinculo<?php echo ($_POST['req']?:'').'-'.($_POST['danid']?:'')?>">
        <thead>
            <tr >
                <?php if(!$readonly and $_REQUEST['xls'] == 0){?>
                <th style="display: none">Ação</th>
                <?php } ?>
                <th>UF</th>
                <?php if($_REQUEST['xls'] == 1) :?>
                <th>Código IBGE</th>
                <?php endif;?>
                <th>Município</th>
                <th>Esfera</th>
                <th>CPF</th>
                <th>Nome</th>
                <th>CRN-UF</th>
                <th>Cargo/Função</th>
                <th>Vinculação</th>
                <th>Data Vinculação</th>
                <?php if($_REQUEST['xls'] == 1) :?>
                    <th>Data de Desvinculação</th>
                    <th>E-mail</th>
                    <th>E-mail alternativo</th>
                    <th>Telefone fixo</th>
                    <th>Telefone celular</th>
                    <th>CEP</th>
                    <th>Logradouro</th>
                    <th>Complemento</th>
                    <th>Número</th>
                    <th>Bairro</th>
                <?php endif;?>
                <th>Situação</th>

            </tr>
        </thead>
        <tbody>
        <?php foreach($arrDados as $vinculo){?>
            <tr id="tr_<?php echo $vinculo['vnid']; ?>"
                <?php echo ($vinculo['vnstatus'] == 'A' && $vinculo['snid'] == 4 ? 'style="background-color:#99ff99"' : ''); ?>
                <?php echo ($vinculo['vnstatus'] == 'I' || $vinculo['snid'] == 5 ? 'style="background-color:#ffcccc"' : ''); ?> >
                <?php if(!$readonly and $_REQUEST['xls'] == 0){?>
                <td style="display: none">
                <?php if($vinculo['acao'] == 't'){ ?>
                    <div>
                        <input type="hidden" id="snid_anterior" value="<?php echo $vinculo['snid']; ?>" />
                        <div class="radio radio-inline">
                            <input
                                class="analisar"
                                vnid="<?php echo $vinculo['vnid']; ?>"
                                name="snid"
                                id="snid-4"
                                type="radio"
                                value="4"
                                <?php echo ($vinculo['snid'] == 4 ? 'checked="checked"' : ''); ?> >
                            <label for="snaceito-t">Aprovado</label>
                        </div>
                        <div class="radio radio-inline" style="margin:0px;">
                            <input
                                class="analisar"
                                vnid="<?php echo $vinculo['vnid']; ?>"
                                name="snid"
                                id="snid-5"
                                type="radio"
                                value="5"
                                <?php echo ($vinculo['snid'] == 5 ? 'checked="checked"' : ''); ?> >
                            <label for="snaceito-f">Reprovado</label>
                        </div>
                    </div>
                <?php } ?>
                </td>
                <?php }?>
                <td><?php echo $vinculo['estuf']; ?></td>
                <?php if($_REQUEST['xls'] == 1) :?>
                <td><?php echo $vinculo['muncod']; ?></td>
                <?php endif;?>
                <td><?php echo $vinculo['mundescricao']; ?></td>
                <td><?php echo $vinculo['esfera']; ?></td>
                <td><?php echo formatar_cpf($vinculo['vncpf']); ?></td>
                <td>
                    <?php if(!$readonly){?>
                        <i
                                class="fa fa-user detalhe"
                                data-toggle="modal"
                                data-target="#modal"
                                danid="<?php	echo ($vinculo['danid']) ? $vinculo['danid'] : $vinculo['vncpf'] ?>"
                                tipo_id="<?php	echo ($vinculo['danid']) ? 'danid' : 'cpf' ?>"
                        ></i>
                    <?php } ?>
                    <?php echo $vinculo['usunome']; ?>
                </td>
                <td><?php echo $vinculo['dan_crn_uf']; ?></td>
                <td><?php echo $vinculo['tendsc']; ?></td>
                <td><?php echo $vinculo['vinculacao']; ?></td>
                <td> <?php echo formata_data($vinculo['vndatavinculacao']); ?></td>
                <?php if($_REQUEST['xls'] == 1) :?>
                    <th><?php echo  formata_data($vinculo['vndataassinaturadesvinculacao']); ?></th>
                    <th><?php echo $vinculo['danemailprincipal']; ?></th>
                    <th><?php echo $vinculo['danemailalternativo']; ?></th>
                    <th><?php echo formatar_telefone($vinculo['dantelefonefixo']); ?></th>
                    <th><?php echo formatar_telefone($vinculo['dantelefonecelular']); ?></th>
                    <th><?php echo formatar_cep($vinculo['endcep']); ?></th>
                    <th><?php echo $vinculo['endlogradouro']; ?></th>
                    <th><?php echo $vinculo['endcomplemento']; ?></th>
                    <th><?php echo $vinculo['endnumero']; ?></th>
                    <th><?php echo $vinculo['endbairro']; ?></th>
                <?php endif;?>
                <td><?php echo $vinculo['sndescricao']; ?></td>

            </tr>
        <?php }?>
        </tbody>
    </table>
</div>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog" id="modal_detalhe" style="width:100%">
    </div>
</div>
<script>
    $(function(){
        $("#listaVinculo<?php echo ($_POST['req']?:'').'-'.($_POST['danid']?:'')?>").DataTable({
            paginate: true,
            searching: true,
            'aoColumnDefs' : [{
                'bSortable' : true,
                'aTargets' : [ 'unsorted' ],
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
    });

</script>
