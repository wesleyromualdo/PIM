<?php

require_once APPRAIZ . 'includes/cabecalho.inc';

echo $simec->title("Guia Planejamento");
?>
<input type="hidden" id="itrid" name="itrid" value="<?php echo $itrid; ?>">
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
    	<?php
		$url = 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A';
    	$abasGuias = array(
    	    0 => array( "descricao" => "Guia Diagnóstico", 	"link" => "par3.php?modulo=principal/configuracao/guia&acao=A" ),
    	    1 => array( "descricao" => "Guia Planejamento", "link" => "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A" ),
    	);
        echo $simec->tab ( $abasGuias, $url );
        echo '<br>';
		$url = 'par3.php?modulo=principal/configuracao/cadPropostaIniciativa&acao=A';
    	$abasGuias = array(
    	    0 => array( "descricao" => "Lista Proposta Iniciativas",   "link" => "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A" ),
    	    1 => array( "descricao" => "Cadastro Proposta Iniciativa", "link" => "par3.php?modulo=principal/configuracao/cadPropostaIniciativa&acao=A" ),
    	);
        echo $simec->tab ( $abasGuias, $url );
		?>
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <div class="ibox-title">
                    <h5>
                        Informações básicas
                    </h5>
                </div>
                <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                    <?php
                    echo $simec->textarea('pindsc', 'Descrição', $dados['inidsc'], array('placeHolder' => '', 'maxlengh' => 1200, $boIndicador));

                    $tiposA = array(
                        "G" => "Global",
                        "E" => "Por escola",
                    );

                    echo $simec->select('pincronograma', 'Cronograma', $dados['pincronograma'], $tiposA, array('placeHolder' => 'Cronograma', 'maxlengh' => 1200));

                    $tiposA = array(
                        "2017" => "2017",
                        "2018" => "2018",
                        "2019" => "2019",
                    );

                    echo $simec->select('pinano[]', 'Anos', array(), $tiposA, array('placeHolder' => 'Anos', 'maxlengh' => 1200, $disabled));

                    $sql = "SELECT
                            	obiid  as codigo,
                            	obidesc  as descricao
                            FROM par3.objetoiniciativa
                            where obistatus = 'A'
                            ORDER BY 2";

                    echo $simec->select('obiid', 'Tipo de Objeto', $dados['obiid'], $sql, array('placeHolder' => 'Objeto Iniciativa', 'maxlengh' => 1200));

                    $sql = "SELECT
                            	aciid as codigo,
                            	acidesc as descricao
                            FROM par3.areascorrelatasiniciativa
                            WHERE acistatus = 'A'
                            ORDER BY 2";

            		echo $simec->select('aciid[]', 'Areas Relacionadas', $dados['aciid'], $sql, array('placeHolder' => 'Areas Relacionadas', 'maxlengh' => 1200));

                    $tiposA = array(
                        "E" => "Estadual",
                        "M" => "Municipal",
                        "D" => "Distrital",
                    );

                    echo $simec->select('esfera[]', 'Esfera', array(), $tiposA, array('placeHolder' => 'Esfera', 'maxlengh' => 1200, $disabled));

                    $tiposA = array(
                        "B" => "Educação Básica",
                        "S" => "Ensino Superior",
                    );

                    echo $simec->select('nivel[]', 'Nível', array(), $tiposA, array('placeHolder' => 'Esfera', 'maxlengh' => 1200, $disabled));

                    $tiposA = array(
                        "I" => "Educação infantil",
                        "F" => "Ensino fundamental",
                        "M" => "Ensino Médio",
                    );

                    echo $simec->select('etapa[]', 'Etapa', array(), $tiposA, array('placeHolder' => 'Esfera', 'maxlengh' => 1200, $disabled));

                    $tiposA = array(
                        "N" => "Ensino Regular",
                        "E" => "Educação especial",
                        "T" => "Ensino tecnológico e profissional",
                        "J" => "Educação de jovens e adultos",
                        "A" => "Educação ambiental",
                        "C" => "Educação do Campo",
                        "R" => "Educação étnico racial",
                        "I" => "Educação indígena",
                        "Q" => "Educação quilombola",
                        "D" => "EAD",
                    );

                    echo $simec->select('modalidade[]', 'Modalidade', array(), $tiposA, array('placeHolder' => 'Esfera', 'maxlengh' => 1200, $disabled));

                    $tiposA = array(
                        "C" => "Campo",
                        "U" => "Urbano",
                        "P" => "Parcial",
                        "I" => "Integral",
                        "P" => "Público",
                        "I" => "Privado",
                    );

                    echo $simec->select('desdobramento[]', 'Desdobramento', array(), $tiposA, array('placeHolder' => 'Esfera', 'maxlengh' => 1200, $disabled));
                    ?>
                    <div class="ibox-title">
                        <h5>
                            Indicadores Vinculados
                        </h5>
                    </div>
                    <?

                    $sql = "SELECT
                            	dimid as codigo,
                            	dimcod||' - '||dimdsc||' ('||itr.itrdsc||')' as descricao
                            FROM par3.dimensao dim
                            INNER JOIN par3.instrumento itr ON itr.itrid = dim.itrid
                            ORDER BY
                            	itr.itrdsc, dimcod";

                    echo $simec->select('dimensao', 'Dimenssão', array(), $sql, array('placeHolder' => 'Dimenssão', 'maxlengh' => 1200, $disabled));

                    $sql = "SELECT
                            	dim.dimid as codigo,
                            	dim.dimcod||'.'||are.arecod||' - '||are.aredsc||' ('||itr.itrdsc||')' as descricao
                            FROM par3.dimensao dim
                            INNER JOIN par3.instrumento 	itr ON itr.itrid = dim.itrid
                            INNER JOIN par3.area 		are ON are.dimid = dim.dimid
                            ORDER BY
                            	itr.itrdsc, dim.dimcod, are.arecod
                            LIMIT 30";

                    echo $simec->select('area', 'Area', array(), $sql, array('placeHolder' => 'Area', 'maxlengh' => 1200, $disabled));

                    $sql = "SELECT
                            	dim.dimid as codigo,
                            	dim.dimcod||'.'||are.arecod||'.'||ind.indcod||' - '||ind.indcod||' ('||itr.itrdsc||')' as descricao
                            FROM par3.dimensao dim
                            INNER JOIN par3.instrumento 	itr ON itr.itrid = dim.itrid
                            INNER JOIN par3.area 		are ON are.dimid = dim.dimid
                            INNER JOIN par3.indicador	ind ON ind.areid = are.areid
                            ORDER BY
                            	itr.itrdsc, dim.dimcod, are.arecod, ind.indcod
                            LIMIT 30";

                    echo $simec->select('indicador', 'Indicador', array(), $sql, array('placeHolder' => 'Indicador', 'maxlengh' => 1200, $disabled));

                    ?>
                    <div class="form-group ">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label"></label>
                        <div class="col-sm-9 col-md-9 col-lg-9 ">
            				<button type="button" class="btn btn-link" onclick="window.location.href='par3.php?modulo=principal/configuracao/cadPropostaIniciativa&acao=A';">
            					<span class="fa fa-plus"> </span> Inserir Indicador
            				</button>
                        </div>
                    </div>
                    <div class="form-group ">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Indicadores: </label>
                        <div class="col-sm-9 col-md-9 col-lg-9 ">
                        </div>
                    </div>
                    <div class="ibox-title">
                        <h5>
                            Itens:
                        </h5>
                    </div>
                    <?

                    $tiposA = array(
                        "C" => "Onibus",
                        "U" => "Micro-Onibus",
                        "P" => "Van",
                        "I" => "Carro",
                        "P" => "Barco",
                        "I" => "Bicicleta",
                    );

                    echo $simec->select('itens[]', 'Itens', array(), $tiposA, array('placeHolder' => 'Itens', 'maxlengh' => 1200, $disabled));
                    ?>
                    <div class="ibox-title">
                        <h5>
                            Disponibilizar por área
                        </h5>
                    </div>
                    <?

                    $sql = "SELECT
                            	estuf as codigo,
                            	estdescricao as descricao
                            FROM territorios.estado
                            ORDER BY 2";

                    echo $simec->select('estados[]', 'Estados', array(), $sql, array('placeHolder' => 'Estados', 'maxlengh' => 1200, $disabled));

                    $sql = "SELECT
                            	gtmid as codigo,
                            	gtmdsc as descricao
                            FROM territorios.grupotipomunicipio
                            WHERE
                            	gtmstatus = 'A'
                            ORDER BY 2";

                    echo $simec->select('grupo_municipio[]', 'Grupo de Municipios', array(), $sql, array('placeHolder' => 'Grupo de Municipios', 'maxlengh' => 1200, $disabled));
                    ?>
                    <div class="form-group">
            			<div class="col-lg-12 col-sm-offset-3">
            				<button type="button" class="btn btn-success">
            					<span class="fa fa-save"> </span> Salvar
            				</button>
            			</div>
            		</div>
                </form>
            </div>
        </div>
    </div>
</div>

<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog">
        <div class="ibox-title">
            <h5 id="html_modal-title"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form">
        </div>
    </div>

    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="ibox-title">
            <h5 id="html_modal-title-large"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form-large">
        </div>
    </div>
</div>

<script>
    function popupGuia(acao, tipo, id, itrid) {
        var titulo = 'Gerenciar dimensão';

        switch (tipo) {
            case 'area':
                titulo = 'Gerenciar área';
                break;
            case 'indicador':
                titulo = 'Gerenciar indicador';
                break;
            case 'criterio':
                titulo = 'Gerenciar componentes do indicador';
                break;
        }

        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupGuia&acao=A',
            data: '&acaoGuia=' + acao + '&tipoGuia=' + tipo + '&codigo=' + id + '&itrid=' + itrid,
            async: false,
            success: function (resp) {
                $('#html_modal-form').html(resp);
                $('#html_modal-title').html(titulo);
                $('#modal-form').modal();
            }
        });
    }
</script>