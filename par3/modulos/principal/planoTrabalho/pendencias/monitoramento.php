<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

$pendenciaBox = $_REQUEST['pendencia'];
$boExists     = $helperPendencia->consultaPendenciaMonitoramentoPAR( $_REQUEST['inuid'] );
$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox,$boExists);

if( $boExists ){

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
            var funcao_detalhe = $(this).attr('id');
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
                    data: '&req=detalharPendencia&tipo=monitoramento&funcao='+funcao_detalhe+'&inuid=<?php echo $_REQUEST['inuid']; ?>',
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
		<div class=" widget  yellow-bg text-left" style="background-color: <?php echo $arAuxHelperPendencia['widget']; ?>;" >
			<div class="slim-scroll">
				<i class="fa <?php echo $arAuxHelperPendencia['thumb']; ?> fa-2x"></i>
				 <span class="font-bold no-margins" style="font-size: 16pt;"><?php echo $arAuxHelperPendencia['description']; ?> </span>

	                		<?php if($boExisteRegraPagamento) {?>
								<p>
									<h3>Processos com pagamento efetivado e Sem nota fiscal</h3>
								</p>
								<div class="list-group">
		                        <?php foreach( $boExisteRegraPagamento as $dados ){ ?>
				                	<a class="list-group-item active" href="#">
					                    <h4 class="list-group-item-heading">Processo Nº: <?php echo $dados['processo'] ; ?> </h4>
			                            <p class="list-group-item-text">Saldo em conta: R$<?php echo number_format($dados['saldoconta'], 2, ',', '.'); ; ?>  </p>
				                    </a>
		                        <?php } ?>
								</div>
							<?php } ?>

	                		<?php if($boExisteSaldoconta) {?>
		                		<h3> &nbsp; Processos com saldo em conta e pendências no monitoramento</h3><br />
								<div class="list-group">
		                        <?php foreach( $boExisteSaldoconta as $dados ){ ?>
				                	<a class="list-group-item active" href="#">
					                    <h4 class="list-group-item-heading">Processo Nº: <?php echo $dados['processo'] ; ?> </h4>
			                            <p class="list-group-item-text">Vigência: <?php echo $dados['fimvigencia'] ; ?>  </p>
				                    </a>
		                        <?php } ?>
								</div>
							<?php } ?>

	                		<?php if($boPendenciaMonitoramento) {?>
		                		<h3> &nbsp;Existem subações 100% pagas com pendências no monitoramento</h3><br />
								<div class="list-group">
								<?php foreach( $arrPendencia['pendencias'] as $k => $pendencia ){ ?>
				                	<a class="list-group-item active detalhar" id="<?php echo $k; ?>" >
					                    <h4 class="list-group-item-heading"><?php echo $pendencia ; ?> <button class="btn btn-success" >Detalhar pendências</button> </h4>
				                    </a>
				                    <div id="div_<?php echo $k; ?>" ></div>
		                        <?php } ?>
								</div>
							<?php } ?>

	                		<?php if($boPendenciaTermo) {?>
		                		<h3> &nbsp;Há termo(s) de compromisso que necessita(m) validação pelo prefeito</h3><br />
								<div class="list-group">
								<?php foreach( $arrPendenciaTermo['pendencias'] as $k => $pendencia ){ ?>
				                	<a class="list-group-item active detalhar" id="<?php echo $k; ?>" >
					                    <h4 class="list-group-item-heading"><?php echo $pendencia ; ?> <button class="btn btn-success" >Detalhar pendências</button> </h4>
				                    </a>
				                    <div id="div_<?php echo $k; ?>" ></div>
		                        <?php } ?>
								</div>
							<?php } ?>
				</div>
        </div>
	</div>
<?php
}else{?>
	<div class="col-lg-12 monitoramento">
		<div class=" widget  yellow-bg text-left" style="background-color: <?php echo $arAuxHelperPendencia['widget']; ?>;">
			<div class="slim-scroll">
				<i class="fa <?php echo $arAuxHelperPendencia['thumb']; ?> fa-2x"></i>

				<span class="font-bold no-margins" style="font-size: 16pt;"><?php echo $arAuxHelperPendencia['description']; ?> </span>

				<p>
				<h3><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h3>
				</p>

				<div class="list-group">
					<a class="list-group-item active" href="#">
						<h4 class="list-group-item-heading"><?php echo $arAuxHelperPendencia['msg']; ?></h4>
					</a>
				</div>
			</div>
		</div>
	</div>
<?php } ?>