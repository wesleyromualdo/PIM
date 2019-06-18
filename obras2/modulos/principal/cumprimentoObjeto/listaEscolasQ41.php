<script>
jQuery(document).ready(function(){
	jQuery(document).on('click', '.codinep', function(){
		var codinep = jQuery(this).val();
		var obrid = <?php echo $_REQUEST['obrid']?>;
	    var param = '&requisicao=atualizaEscolaObra&obrid='+obrid+'&codinep='+codinep;
	    jQuery.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: param,
       		async: false,
       		success: function(resp){
       			jQuery('#escola').html(resp);
                jQuery('#modal-form').dialog('close');
       		}
     	});
    });
});
</script>
<?php
global $db;

$sql = "SELECT
        	obr.obrid,
        	emp.empesfera,
        	m.muncod,
        	est.estcod,
        	co_inep
        FROM obras2.obras obr
        INNER JOIN obras2.empreendimento emp ON emp.empid = obr.empid
        INNER JOIN entidade.endereco ende ON ende.endid = emp.endid
        INNER JOIN territorios.municipio m ON m.muncod = ende.muncod
        INNER JOIN territorios.estado est ON est.estuf = m.estuf
        WHERE obr.obrid = {$_REQUEST['obrid']}";

$arrDadosObra = $db->pegaLinha($sql);

$where = array(
            // 'tp_situacao_funcionamento = 1',
            'tp_categoria_escola_privada IS NULL',
         );


$where[] = "co_municipio = '{$arrDadosObra['muncod']}'";

$sql = "
        SELECT
            '<input type=radio name=codinep class=codinep value=\'0\' '||
	        CASE WHEN '{$arrDadosObra['co_inep']}' = '0'
                THEN 'checked=checked'
	            ELSE ''
	        END ||' />' as chk,
            00000000 co_entidade,
            'No momento, não possui código INEP' no_entidade

        UNION

        SELECT
        	'<input type=radio name=codinep class=codinep value='|| co_entidade ||' '||
	        CASE WHEN obr.co_inep IS NOT NULL
                THEN 'checked=checked'
	            ELSE ''
	        END ||' />' as chk,
            co_entidade,
            no_entidade
        FROM
        	educacenso_2015.ts_censo_basico_escola esc
	    LEFT JOIN obras2.obras obr ON obr.co_inep = esc.co_entidade
        WHERE ".implode(' AND ', $where)." ORDER BY no_entidade";

$cabecalho = array('&nbsp', 'Código INEP','Escola');
$db->monta_lista_simples($sql, $cabecalho, 5000, 5, 'N', 'center', 'N', "formulario", '', '');
?>