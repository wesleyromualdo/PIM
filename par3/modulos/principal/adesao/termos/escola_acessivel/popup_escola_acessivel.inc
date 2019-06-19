<?php
include_once APPRAIZ . "par3/classes/model/adesao_programa/EscolasEscolaAcessivel.class.inc";
include_once APPRAIZ . "par3/classes/model/adesao_programa/Adesaoescolaacessivel.class.inc";
include_once APPRAIZ . "par3/classes/controller/adesao_programa/escolaAcessivelPopUpEscola.inc";
$pfaid = 12; // escola acessivel
?>
    <script>
        $(document).ready(function () {
            //$("#selecionar_todos").closest('th').removeClass().find('div').remove();

            if ($("#tipoPesquisa2").val() == 'lista') {
                window.location.hash = '#resultado';
            }
            $('.ordenavel').click(function () {
                var campo = $(this).attr('data-campo-ordenacao');
                $('#ordenacao').val(campo);
                if (campo != 'acao') {
                    $('.loading-dialog-par3').show();
                    $('#filtrar_pesquisa').click();
                }
                $('.loading-dialog-par3').hide();
            });

            $("#desmarcar_todos").on('click', function () {
                var vlr = '';
                //$(".escola_input").prop('checked', false);
                //$(".escola_input").prop('checked', $("#selecionar_todos").is(':checked'));
                $(':checkbox').prop('checked', false).removeAttr('checked');
                $("#valor_saldo_disponivel").val('0');
                var vduf = $('#valor-disponivel-uf').html();
                var vdmun = $('#valor-disponivel-mun').html();
                if(vduf){
                    vlr = vduf;
                } else {
                    vlr = vdmun;
                }
                $("#saldo-total-programa-formatado").html('R$ '+ vlr);
            });            
            
            $('.navbar-listagem').remove();
        });
    </script>

    <div class="ibox-content">
        <form method="post" name="formulario_escolas" id="formulario_escolas" class="form form-horizontal">
            <input type="hidden" value="salvar_vinculo_escola" id="requisicao" name="requisicao"/>
            <input type="hidden" value="<?php echo $_POST['apta']; ?>" id="apta" name="apta"/>
            <input type="hidden" value="<?php echo $adpid; ?>" id="adpid" name="adpid"/>
            <input type="hidden" value="<?php echo $_REQUEST['ordenacao'] ?>" id="ordenacao" name="ordenacao"/>
            <input type="hidden" value="<?php echo $tipoPesquisa ?>" id="tipoPesquisa2" name="tipoPesquisa2"/>
            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Estado</label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $objTerritorio = new Territorios_Model_Estado();
                $sql = $objTerritorio->pegarSQLSelect($_REQUEST);
                $arrAttr = array('data-placeholder' => 'Estado', 'disabled' => 'disabled');
                echo $simec->select('estuf', null, $estuf, $sql, $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">Município</label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $mundescricao = simec_htmlentities($_REQUEST['mundescricao']);
                // Caso estadual
                if ($itrid == 1 || $itrid == 3) {
                    $valorDisp = $objEscolasEscolaAcessivel->valorDisponivelUF($arrayParams['estuf'], $pfaid);
                    $arrAttr = array('placeHolder' => 'Municipio', 'maxlength' => '100');
                    $default = $_REQUEST['eeamunicipio'];
                } else {
                    $valorDisp = $objEscolasEscolaAcessivel->valorDisponivelMun($arrayParams['muncod'], $pfaid);
                    $arrAttr = array('placeHolder' => 'Municipio', 'maxlength' => '100', 'disabled' => 'disabled', 'id' => 'eeamunicipio');
                    $default = $local;
                }
                $calculoDisp = ($valorDisp - $dadosValorChecados);
                echo $simec->input('eeamunicipio', null, $default, $arrAttr);
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
                    'id' => 'eearede',
                    'disabled' => 'disabled'
                );

                echo $simec->select('eearede', null, $rede, $arrOp, $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Código INEP </label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $arrAttr = array('placeHolder' => 'Código INEP', 'maxlength' => '12', 'id' => 'eeacod');
                echo $simec->input('eeacod', null, $_REQUEST['eeacod'], $arrAttr);
                ?>
            </div>

            <label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1"> Nome da Escola </label>
            <div class="col-sm-8 col-md-8 col-lg-8 ">
                <?php
                $arrAttr = array('placeHolder' => 'Nome da Escola', 'maxlength' => '200', 'id' => 'eeanome');
                echo $simec->input('eeanome', null, $_REQUEST['eeanome'], $arrAttr);
                ?>
            </div>    
            <input type="hidden" value="<?php $calculoSaldoDisp = ($valorDisp - $dadosValorChecados);  echo $calculoSaldoDisp; ?>" id="valor_saldo_disponivel" name="valor_saldo_disponivel"/>
      
            
            <div id="exibe_filtro_escola_selecionada">
                <label class="col-sm-4 col-md-4 col-lg-4 control-label m-b-md" for="resp1"> Escola Selecionada</label>
                <div class="col-sm-8 col-md-8 col-lg-8 ">
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-primary btn-campo">
                            <input name="eeaselecionada" id="selecionada" type="radio" value="S">Sim
                        </label> 
                        <label class="btn btn-primary btn-campo">
                            <input name="eeaselecionada" id="naoselecionada" type="radio" value="N">Não
                        </label>
                    </div>            
                </div>
            </div>
            <center>
                <button type="button" id="filtrar_pesquisa"
                        onclick="filtraPesquisaEscolaAcessivel( <?php echo $inuid; ?>, <?php echo $adpid ?>, <?php echo $adpano_ciclo; ?>, <?php echo $prgid; ?>, 'lista' )"
                        class="btn btn-success salvar" <?php echo $disabled; ?>>
                    <i class="fa fa-save"></i> Filtrar Pesquisa
                </button>
            </center>

            <hr class="divider">
            <?php if ($calculoDisp && $calculoDisp < 0 ) { ?>
            <div class="alert alert-info">
                <strong>Prezado(a) Secretário(a),</strong><br>A seleção de escolas de seu município excedeu o limite orçamentário previsto para sua rede. Para ajustar o número de escolas ao valor disponibilizado será (serão) mantida(s) as escolas pré-selecionadas pelo MEC.<br>Aguarde novo período de adesão!
            </div>
            <?php } ?>
            <div class="row" id="exibe_valor_programa">
                <?php if ($itrid == 1 || $itrid == 3) { ?>
                <div class="col-md-3">
                    <label>Valor disponível (Estado):</label>
                    <h2>R$ <span id="valor-disponivel-uf"><?php echo simec_number_format($valorDisp, 2, ',', '.' ); ?></span></h2>
                    <input id="valor_disponivel_uf" name="valor_disponivel_uf" type="hidden" value="<?php echo $valorDisp; ?>">
                </div>                
                <?php } else { ?>
                <div class="col-md-3">
                    <label>Valor disponível (Município):</label>
                    <h2>R$ <span id="valor-disponivel-mun"><?php echo simec_number_format($valorDisp, 2, ',', '.' ); ?></span></h2>
                    <input id="valor_disponivel_mun" name="valor_disponivel_mun" type="hidden" value="<?php echo $valorDisp; ?>">
                </div>                
                <?php } ?>
                <div class="col-md-3">
                    <label>Saldo disponível:</label>
                    <h2 id="saldo-total-programa-formatado">R$ <?php echo simec_number_format($calculoDisp, 2, ',', '.' );?></h2>
                </div>
            </div>
            <?php
                $modelAdesaoPrograma = new Par3_Model_AdesaoPrograma();
                $result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid, $_SESSION['par3']['adpano_ciclo']);

                $docidInserido = $result['docid'];
                $situacaoAtual = wf_pegarEstadoAtual($docidInserido);
                $esdid = $situacaoAtual['esdid'];
            ?>
            <?php if ($esdid == WF_ESDID_EMCADASTRAMENTO_ESCOLAACESSIVEL) { ?>            
                <div id="btn-desmarcar-todos" class="row">
                    <div class="col-md-12">
                        <button class="btn btn-sm btn-primary" type="button" name="desmarcar_todos" id="desmarcar_todos" title="Desmarcar todos">Desmarcar todos</button>
                    </div>
                </div>
            <?php } ?>
            <div id="resultado" class="m-t-md">                
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
            
            if ((!$bloqueiaGeral) && ($save) && ($_POST['apta'] == 'true') && $esdid == WF_ESDID_EMCADASTRAMENTO_ESCOLAACESSIVEL) {
                ?>
                <center>
                    <button type="button" class="btn btn-primary salvar_escolas" onclick="submeteFormEscolaAcessivel()"
                        <?php echo $disabled; ?>><i class="fa fa-save"></i> Salvar
                    </button>
                </center>
                <?php
            }
            ?>
        </form>
    </div>
    
    <script>
        <?php if ($calculoDisp && $calculoDisp < 0 ) { ?>
            $(document).ready(function () {
                swal({
                    title: "Atenção!",
                    text: "Prezado(a) Secretário(a),<br><br>A seleção de escolas de seu município excedeu o limite orçamentário previsto para sua rede. Para ajustar o número de escolas ao valor disponibilizado será (serão) mantida(s) as escolas pré-selecionadas pelo MEC.<br><br>Aguarde novo período de adesão!",
                    type: "info", 
//                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55", 
                    confirmButtonText: "Estou Ciente",
                    closeOnConfirm: "on",
//                    cancelButtonText: "Cancelar",
                    html: true
                },
                function (isConfirm) {
                    console.log('estou ciente');
                });
            });        
        <?php } ?>
    </script>

<?php die(); ?>