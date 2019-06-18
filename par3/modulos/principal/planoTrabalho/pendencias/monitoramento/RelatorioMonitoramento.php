<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();
$RelatorioMonitoramento = new Par3_Controller_RelatorioMonitoramento();

/*$pendenciaBox = $_REQUEST['pendencia'];
$boExists     = $helperPendencia->consultaPendenciaMonitoramentoPAR( $_REQUEST['inuid'] );
$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox,$boExists);*/
//$_REQUEST['relatorioListado'];

    $oPendencia = new Par3_Model_Pendencias();

    $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
    $estuf = $instrumentoUnidade->estuf;
    $muncod = $instrumentoUnidade->muncod;
    $itrid = $instrumentoUnidade->itrid;

    $boExisteRegraPagamento = $oPendencia->recuperaProcessosRegraPagamento($_REQUEST['inuid'], $estuf, $muncod , $itrid );

    $boExisteSaldoconta = $oPendencia->recuperaProcessosRegraSaldoConta($_REQUEST['inuid'], $estuf, $muncod , $itrid );

    $arrPendencia = $oPendencia->recuperaProcessosRegraMonitoramento2010($_REQUEST['inuid']);
    $boPendenciaMonitoramento = count($arrPendencia['pendencias']) > 0 ? true : false;

    $arrPendenciaTermo = $oPendencia->recuperaProcessosRegraMonitoramento2010Termos($_REQUEST['inuid']);
    $boPendenciaTermo = count($arrPendenciaTermo['pendencias']) > 0 ? true : false;

    ?>
    <script>
    $(document).ready(function(){
        $('.detalhar').click(function(){
            var funcao_principal = $(this).attr('id');
            var funcao_detalhe = $(this).attr('data');
            var conteudo = $('#div_'+funcao_detalhe).html();

            if(conteudo != ''){
                var display = $('#div_'+funcao_detalhe).css('display');
                if(display == 'none'){
                    $('#div_'+funcao_detalhe).show();
                }else{
                    $('#div_'+funcao_detalhe).hide();
                }
            }else{
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: '&req=detalharPendencia&tipo=monitoramento&funcao='+funcao_principal+'&inuid=<?php echo $_REQUEST['inuid']; ?>',
                    async: true,
                    beforeSend: function () {
                        $('#loading').show();
                    },
                    success: function (resp) {
                        $('#loading').hide();
                        if(resp != ''){
                            $('#div_'+funcao_detalhe).html(resp);
                        }
                    }
                });
            }
        });
    });
    </script>
    <div class="col-lg-12 monitoramento">
        <div>
            <div class="slim-scroll">
                <i class="fa-2x"></i>
                 <span class="font-bold no-margins"></span>
                    <?php
//                     ver($boExisteRegraPagamento, $_REQUEST, d);
                    switch($_REQUEST['relatorioListado']){
                        case 'RegraPagamento':
                            if($boExisteRegraPagamento) {
                                $sql = $oPendencia->recuperaProcessosRegraPagamento($_REQUEST['inuid'], $estuf, $muncod , $itrid, true);
                                $arrPost['sql'] = $sql;
                                $arrPost['cabecalho'] = array('Processo Nº','Saldo em conta');
                                $arrPost['esconderColunas'] = array('muncod','estuf','saldo','valorpago','saldopercentual','empenho');
                                $RelatorioMonitoramento->listaMonitoramentoDetalhado($arrPost);
                             }
                             break;
                        case 'SaldoConta':
                            if($boExisteSaldoconta) {
                            ?>
                                <div class="list-group">
                                    <?php
                                        $sql = $oPendencia->recuperaProcessosRegraSaldoConta($_REQUEST['inuid'], $estuf, $muncod , $itrid, true);
                                        $arrPost['sql'] = $sql;
                                        $arrPost['cabecalho'] = array('Processo Nº','Vigência');
                                        $arrPost['esconderColunas'] = array('estuf', 'muncod', 'dopid', 'empenho');
                                        $RelatorioMonitoramento->listaMonitoramentoDetalhado($arrPost);
                                    ?>
                                </div>
                        <?php
                            }
                            break;
                        case 'PendenciaMonitoramento':
                            if($boPendenciaMonitoramento) {
                        ?>
                                <div class="list-group">
                                <?php
                                 foreach( $arrPendencia['pendencias'] as $k => $pendencia ){ ?>
                                    <a class="list-group-item detalhar" id="<?php echo $k; ?>" data='<?php echo $k.'_'.date('i_s'); ?>' >
                                        <h4 class="list-group-item-heading"><?php echo $pendencia ; ?> <div class="btn btn-success" >Detalhar pendências</div> </h4>
                                    </a>
                                    <div id="div_<?php echo $k.'_'.date('i_s'); ?>" ></div>
                                <?php } ?>
                                </div>
                        <?php
                            }
                            break;
                        case 'PendenciaTermo':
                            if($boPendenciaTermo) {?>
                                <div class="list-group">
                                <?php foreach( $arrPendenciaTermo['pendencias'] as $k => $pendencia ){ ?>
                                    <a class="list-group-item detalhar" id="<?php echo $k; ?>" >
                                        <h4 class="list-group-item-heading"><?php echo $pendencia ; ?> <button class="btn btn-success" >Detalhar pendências</button> </h4>
                                    </a>
                                    <div id="div_<?php echo $k; ?>" ></div>
                                <?php } ?>
                                </div>
                        <?php
                            }
                            break;
                        } //FECHA SWITCH CASE?>
                </div>
        </div>
    </div>
