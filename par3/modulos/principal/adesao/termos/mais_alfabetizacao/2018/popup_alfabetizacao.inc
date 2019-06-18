<?php
include_once APPRAIZ . "par3/classes/model/adesao_programa/EscolasMaisAlfabetizacao.class.inc";
include_once APPRAIZ . "par3/classes/model/adesao_programa/Adesaoalfabetizacao.class.inc";
include_once APPRAIZ . "par3/classes/controller/adesao_programa/maisAlfabetizacaoPopUpEscola.inc";
?>
    <script>
        $(document).ready(function () {
            $("#selecionar_todos").closest('th').removeClass().find('div').remove();

            if ($("#tipoPesquisa2").val() == 'lista') {
                window.location.hash = '#resultado';
            }
            $('.ordenavel').click(function () {
                var campo = $(this).attr('data-campo-ordenacao');
                $('#ordenacao').val(campo);
                if (campo != 'acao') {
                    $('#filtrar_pesquisa').click()
                }
            });

            $("#selecionar_todos").on('click', function () {
                $(".escola_input").prop('checked', $("#selecionar_todos").is(':checked'));
            })
        });
    </script>

    <div class="ibox-content">
        <form method="post" name="formulario_escolas" id="formulario_escolas" class="form form-horizontal">
            <input type="hidden" value="salvar_vinculo_escola" id="requisicao" name="requisicao"/>
            <input type="hidden" value="N" id="carga_horaria_form" name="carga_horaria_escolas"/>
            <input type="hidden" value="<?php echo $adpid; ?>" id="adpid" name="adpid"/>
            <input type="hidden" value="<?php echo $carga_horaria; ?>" id="carga_horaria" name="carga_horaria"/>
            <input type="hidden" value="<?php echo $_REQUEST['ordenacao'] ?>" id="ordenacao" name="ordenacao"/>
            <input type="hidden" value="<?php echo $tipoPesquisa ?>" id="tipoPesquisa2" name="tipoPesquisa2"/>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Estado</label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $sql = Territorios_Model_Estado::pegarSQLSelect($_REQUEST);
                $arrAttr = array('data-placeholder' => 'Estado', 'disabled' => 'disabled');
                echo $simec->select('estuf', null, $estuf, $sql, $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">Município</label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $mundescricao = simec_htmlentities($_REQUEST['mundescricao']);
                // Caso estadual
                if ($itrid == 1) {
                    $arrAttr = array('placeHolder' => 'Municipio', 'maxlength' => '100');
                    $default = $_REQUEST['emamunicipio'];
                } else {
                    $arrAttr = array('placeHolder' => 'Municipio', 'maxlength' => '100', 'disabled' => 'disabled', 'id' => 'emamunicipio');
                    $default = $local;
                }

                echo $simec->input('emamunicipio', null, $default, $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Dependência Administrativa </label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $arrOp = array(
                    'Estadual' => 'Estadual',
                    'Municipal' => 'Municipal'
                );

                if ($itrid == 1 || $itrid == 3) {
                    $rede = 'Estadual';
                } else {
                    $rede = 'Municipal';
                }

                $arrAttr = array(
                    'data-placeholder' => 'Dependência Administrativa',
                    'id' => 'emarede',
                    'disabled' => 'disabled'
                );

                echo $simec->select('emarede', null, $rede, $arrOp, $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Código INEP </label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $arrAttr = array('placeHolder' => 'Código INEP', 'maxlength' => '12', 'id' => 'emacod');
                echo $simec->input('emacod', null, $_REQUEST['emacod'], $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Nome da Escola </label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $arrAttr = array('placeHolder' => 'Nome da Escola', 'maxlength' => '200', 'id' => 'emanome');
                echo $simec->input('emanome', null, $_REQUEST['emanome'], $arrAttr);
                ?>
            </div>

            <?php
            if ($carga_horaria != 'N') :
                ?>
                <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Selecionadas </label>
                <div class="col-sm-8 col-md-8 col-lg-8 ">
                    <?php
                    $arrOp = array('' => '', 'S' => 'Sim', 'N' => 'Não');
                    $arrAttr = array('data-placeholder' => 'Selecionadas', 'id' => 'checado'
                    );
                    echo $simec->select('checado', null, $_REQUEST['checado'], $arrOp, $arrAttr);
                    ?>
                </div>

                <hr class="divider">

            <?php endif; ?>

            <center>
                <button type="button" id="filtrar_pesquisa"
                        onclick="filtraPesquisaAlfabetizacao( <?php echo $inuid; ?>, <?php echo $adpid ?>, <?php echo $adpano_ciclo; ?>, '<?php echo $carga_horaria; ?>', <?php echo $prgid; ?>, 'lista' )"
                        class="btn btn-success salvar" <?php echo $disabled; ?>>
                    <i class="fa fa-save"></i> Filtrar Pesquisa
                </button>
            </center>

            <hr class="divider">

            <?php if($carga_horaria != 'N'): ?>
                <div class="alert alert-warning"> Foram consideradas apenas turmas que possuam no mínimo 10 matrículas de 1º ou 2º
                    ano do ensino fundamental, conforme dados do Censo
                    Escolar 2017.
                </div>
            <?php else: ?>
                <div class="alert alert-warning"> Escolas que não possuem pelo menos 1 turma com no mínimo 10 matrículas de 1º ou 2º ano do ensino fundamental,
                    conforme dados do Censo Escolar 2017.
                </div>
            <?php endif;?>

            <div id="resultado">
                <?php
                $save = false;
                if ($listaInfoEscolas['status'] == 'success') {
                    $save = true;
                    $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
                } else {
                    ?>
                    <div style="" class="text-center " id="tb_render"><b>Nenhum registro encontrado</b></div>
                    <?php
                } ?>
            </div>

            <?php

            $modelAdesaoPrograma = new Par3_Model_AdesaoPrograma();
            $result = $modelAdesaoPrograma->recuperaPorInuid($inuid, 10,$_SESSION['par3']['adpano_ciclo']);

            $docidInserido = $result['docid'];
            $situacaoAtual = wf_pegarEstadoAtual($docidInserido);
            $esdid = $situacaoAtual['esdid'];

            if ((!$bloqueiaGeral) && ($carga_horaria != 'N') && ($save) && $esdid == WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO) {
                ?>
                <center>
                    <button type="button" class="btn btn-success salvar" onclick="submeteFormEscolasAlfabetizacao()"
                        <?php echo $disabled; ?>><i class="fa fa-save"></i> Salvar
                    </button>
                </center>
                <?php
            }
            ?>
        </form>
    </div>

<?php die(); ?>