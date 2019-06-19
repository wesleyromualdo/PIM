<?php 
$prp = new Par3_Model_ProadesaoRespostasPnfcd();
$ppp = $prp->pegarRespostasPorInuid($_GET['inuid']);
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

if ($_GET['req']) {
	switch ($_REQUEST['req']) {
		case 'downloadArquivo':
			include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
			$file = new FilesSimec("proadesao_respostas_pnfcd ", null, "par3");
			$file->getDownloadArquivo($_REQUEST['arqid']);
			break;
		case 'excluirArquivo':
			$url = "par3.php?modulo=principal/adesao/termo&acao=A&inuid={$_GET['inuid']}&prgid={$_GET['prgid']}";
			try {
				include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
				$prp = new Par3_Model_ProadesaoRespostasPnfcd($_REQUEST['pppid']);
				$prp->pppresp1arqid = null;
				$prp->alterar(array('pppresp1arqid'));
				$prp->commit();
				
				$file = new FilesSimec("proadesao_respostas_pnfcd ", null, "par3");
		    	$file->excluiArquivoFisico($_REQUEST['arqid']);
		    	
		    	simec_redirecionar($url, 'success');
			} catch (Exception $e) {
				simec_redirecionar($url, 'error', $e->getMessage());
			}
		default:
			break;
	}
}

if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {
		
	if( $programa['bovenceu'] == 'S' ){
		$mostrar = 0;
	} else {		
		$mostrar = 1;
	}		
} else {
	$mostrar = 0;
}

if ($itrid == 2)
{
	$proadesaoDados['pfatermomunicipio'] =  str_replace('\"','"', $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#nome_entidade#', $inunome, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermomunicipio']);

	$termoPreenchido = $proadesaoDados['pfatermomunicipio'];
}
else
{
	$proadesaoDados['pfatermoestado'] =  str_replace('\"','"', $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#nome_entidade#',$inunome, $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermoestado']);

	$termoPreenchido = $proadesaoDados['pfatermoestado'];
}

if ($_GET['aceite'] == 'S' || $_GET['aceite'] == 'N')
{
    $dados['inuid'] = $_GET['inuid'];
    $dados['prgid'] = $_GET['prgid'];
    $dados["pfaid"] = $pfaid;

    if (empty($pfaid) && !empty($_GET['pfaid'])) {
        $dados['pfaid'] = $_GET['pfaid'];
    }

    $dados['adpresposta'] = $_GET['aceite'];
    $dados['tapid'] = 1;

    if (empty($proadesaoDados['pfaano'])) {
        $dados['adpano']= date('Y');
    } else {
        $dados['adpano']= $proadesaoDados['pfaano'];
    }

    $dados['usucpf'] = $_SESSION['usucpf'];
    $dados['adpdataresposta'] = date('Y-m-d');

    if ($_GET['aceite'] == 'S') {
        $dados['padtermopreenchido'] = $termoPreenchido;
    }

    if (!empty($aceite['adpid'])) {
        $dados['adpid'] = $aceite['adpid'];
    }

    $modelAdesaoPrograma->popularDadosObjeto($dados);
    $modelAdesaoPrograma->salvar();
    $modelAdesaoPrograma->commit();

    if ($pfaid == 3)
    {
    	$prp =  new Par3_Model_ProadesaoRespostasPnfcd();
    	$ppp = $prp->pegarRespostasPorInuid($_GET['inuid']);
    	$arDados = array();

    	if (empty($_POST['pppid'])) {
    		require_once APPRAIZ . 'includes/workflow.php';
    		$arDados['docid'] = wf_cadastrarDocumento(258, 'Documento de Adesão');
    		$arDados['inuid'] = $_GET['inuid'];
    		$prp->popularDadosObjeto($arDados);
    		$prp->salvar();
    		$prp->commit();
    	}
    }

    if ($_GET['aceite'] == 'S') {
    	$msgAlert = $proadesaoDados['pfamsgdeaceitacao'];
    } else {
    	$msgAlert = $proadesaoDados['pfamsgdenaoaceitacao'];
    }

    $_SESSION[$_POST['pppid']]['termoaceito'] = true;

    if ($_GET['aceite'] == 'N') {
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid='.$dados['inuid'], 'success', $msgAlert);
    } else {
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$dados['inuid']."&prgid=".$dados['prgid'], 'success', $msgAlert);
    }
}
?>
<div class="wrapper wrapper-content animated fadeIn" xmlns="http://www.w3.org/1999/html">
    <div class="row">
		<div class="ibox">
        	<div class="ibox-title">
            	<h3>Termo de adesão</h3>
			</div>
            <div class="ibox-content" style="padding:40px">
            	<div class="tabs-container">
                	<ul class="nav nav-tabs">
                    	<li class="<?php if ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active"; ?>"><a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                        <li class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Orientações</a></li>
                        <?php if ($aceite['adpresposta'] == "S") : ?>
                        	<li class="<?php if (!empty($aceite)) echo "active"?>" ><a data-toggle="tab" href="#tab-3" aria-expanded="false">Formulário</a></li>
						<?php endif; ?>
					</ul>
                    <div class="tab-content">
                    	<div id="tab-1" class="tab-pane <?php if($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active"?>">
                        	<div class="panel-body">
                    			<div style="height: 300px;overflow-y: scroll" id="termo">
				                    <?php if (empty($aceite) && !isset($_SESSION[$_POST['pppid']]['termoaceito'])): ?>
				                        <script>
				                            swal({
				                                title: "Bem-Vindo ao Programa <?php echo $programa['prgdsc'];?>!",
				                                text: '<?php echo str_replace(array("\n", "\r"), '', $proadesaoDados['pfamsgboasvindas']); ?>',
				                                html: true
				                            });
				                        </script>
				                    <?php endif; ?>

				                    <?php
					                    if ($aceite['adprrdposta'] == "S") {
					                        echo $aceite['padtermopreenchido'];
					                    } else {
					                        if (!$termoincompleto) {
					                            echo $termoPreenchido;
					                        } else {
					                            echo "<div class='alert alert-danger'>
					                                    É necessário o cadastramento do Dirigente Municipal de Educação na aba Dados da Unidade no PAR para gerar o Termo de Adesão.
					                                  </div>";
					                        }
					                    }
									?>
								</div>
								<?php if (empty($aceite) && !$termoincompleto) { ?>

		                            <div style="text-align: center;margin-top: 20px;">
		                                <?php if ($mostrar) { ?>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&prgid=1&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>">
		                                        <button class="btn btn-primary">Aceito</button>
		                                    </a>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&prgid=1&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>">
		                                        <button class="btn btn-danger">Não Aceito</button>
		                                    </a>
		                                <?php } ?>
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
		                            </div>

                        		<?php } elseif ($aceite['adpresposta'] == "S" && !$termoincompleto) { ?>
                        			<div style="margin-top: 5px;" class="text-right">
										<label class="label label-primary">
											Assinado por <?php echo formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
										</label>
									</div>
			                        <div style="text-align: center;margin-top: 20px;">
			                           <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&itrid=<?php echo $itrid?>&imprimir=1&inuid=<?php echo $_GET['inuid']?>&prgid=<?php echo $_GET['prgid']?>" target="_blanck">
			                               <button class="btn btn-primary imprimir">Imprimir</button>
			                           </a>
			                           <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
			                        </div>

                    			<?php } else if (!$termoincompleto) { ?>

		                            <div style="text-align: center;margin-top: 20px;">
		                                <?php if ($mostrar) { ?>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&prgid=1&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpid=<?php echo $aceite['adpid'] ?>">
		                                        <button class="btn btn-primary">Aceito</button>
		                                    </a>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&prgid=1&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>">
		                                        <button class="btn btn-danger">Não Aceito</button>
		                                    </a>
		                                <? } ?>
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
		                            </div>

                        		<?php } ?>
                			</div>
                        </div>
	                    <div id="tab-2" class="tab-pane">
							<div class="panel-body">
	                        	<?php echo $proadesaoDados['pfasobre'] ?>
							</div>
	                    </div>
						<div id="tab-3" class="tab-pane <?php if( $aceite['adpresposta'] == "S") echo "active" ?>">
							<div class="panel-body">
								<div class="col-md-11">
                                	<form method="post" name="formulario" id="formulario" class="form form-horizontal" enctype='multipart/form-data'>
										<input type="hidden" name="salvar_pergunta_ppp" value="1">
                                        <input type="hidden" name="prgid" value="<?php echo $_GET['prgid']?>">
                                        <input type="hidden" name="pppid" value="<?php echo $ppp['pppid']?>">
                                        <input type="hidden" name="inuid" value="<?php echo $_GET['inuid']?>">
                                        <?php
                                        	$radioStatus = array('A' => 'Sim', 'I' => 'Não');
											$respostas1 = array('1' => 'A ser elaborada', '2' => 'Em elaboração','3' =>'Já no legislativo');
											$respostas2 = array('1' => '2 anos', '2' => '3 anos','3' =>'4 anos', '4' => 'Outro', '5' => 'Não se aplica');
											$respostas3 = array('1' => '2016', '2' => '2017','3' =>'2018','4' =>'2019','5' =>'Outro','6' =>'Não se aplica');

											if ($ppp['pppresp0'] == 'A' ) {
                                               $validacheck = 'A';
											} else {
												$validacheck = 'I';
											}
										?>
                                        <?php echo $simec->radio('pppresp0', 'Já possui lei disciplinando a gestão democrática da educação pública conforme art. 9º do PNE?',$validacheck, $radioStatus, array('required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));?>
										<?php if (!empty($ppp['pppresp1arqid'])) { ?>
                                            <div class="form-group">
                                                <label for="arqid" class="col-sm-3 col-md-3 col-lg-3 control-label">Lei: </label>
                                                <div class="col-sm-9 col-md-9 col-lg-9">
                                                	<div class="input-group" style="margin-top: 4px">
                                                		<a style='padding: 0px;' class='btn'
                                                		   href='par3.php?modulo=principal/adesao/termo&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>&req=downloadArquivo&arqid=<?php echo $ppp['pppresp1arqid']; ?>'
                                                		   target='blank'>
															<small style='font-size: 12px;'>
																<i class='fa fa-download'></i> Abrir Anexo
															</small>
														</a>
														<?php $estado = wf_pegarEstadoAtual($ppp['docid']); ?>
														<?php if ($estado['esdid'] == PAR3_WORKFLOW_FLUXO_DO_PAR_CADASTRAMENTO || $estado['esdid'] == PAR3_WORKFLOW_FLUXO_DO_PAR_DILIGENCIA) : ?>
															<small>&nbsp;|&nbsp;</small> 
															<a style='padding: 0px;' class='btn danger'
	                                                		   href='par3.php?modulo=principal/adesao/termo&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>&req=excluirArquivo&arqid=<?php echo $ppp['pppresp1arqid']; ?>&pppid=<?php echo $ppp['pppid']; ?>'>
																<small style='font-size: 12px;'>
																	<i class='fa fa-trash'></i> Excluir Anexo
																</small>
															</a>
														<?php endif; ?>
                                                	</div>
                                                </div>
                                            </div>
                                        <?php } else { ?>
                                        	<?php echo $simec->input('pppresp1arqid', 'lei', null, array('type' => 'file','help' => 'Lei que disciplina a gestão democrática da educação pública conforme art 9º do PNE')); ?>
                                        <?php } ?>
                                        <?php echo $simec->select("pppresp1", 'Em que fase se encontra?', $ppp['pppresp1'], $respostas1,null); ?>
                                        <?php echo $simec->select("pppresp2", 'Qual o período de mandato de direção escolar disciplinado pela rede?', $ppp['pppresp2'], $respostas2, array("required")); ?>
                                        <?php echo $simec->input('pppresp2outros', 'Período de mandato de direção escolar',  $ppp['pppresp2outros'], array());?>
                                        <?php echo $simec->select("pppresp3", 'Quando haverá a próxima seleção de diretores?',$ppp['pppresp3'], $respostas3, array("required")); ?>
                                        <?php echo $simec->input('pppresp3outros', 'Ano para seleção de diretores',  $ppp['pppresp3outros'], array());?>
                                        <div class="ibox-footer">
											<div class="row">
												<div class="col-lg-3"></div>
												<div class="col-lg-9">
													<div>
										<?php 		if( $programa['bovenceu'] == 'N' ){?>
														<button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
										<?php 		} else {?>
														<span style="color: red;">Este programa está vencido desde <?php echo formata_data($programa['pfadatafinal']); ?>. Impossibilitando o salvamento do formulário.</span>
										<?php 		}?>
													</div>
												</div>
											</div>
										</div>
									</form>
								</div>
								<div class="col-md-1">
									<?php if (!empty($ppp['docid'])) { ?>
									<?php wf_desenhaBarraNavegacao($ppp['docid'], array('inuid' => $_GET['inuid'])); ?>
									<?php } ?>
								</div>
							</div>
						</div>
                    </div>
    			</div>
			</div>
		</div>
	</div>
</div>
<script>
$(document).ready(function() {
   	<?php if($ppp['pppresp2'] != 4){?>
       	$("#pppresp2outros").parent().parent().hide();
	<?php }?>

    <?php if($ppp['pppresp3'] != 5){?>
       	$("#pppresp3outros").parent().parent().hide();
    <?php }?>

    <?php if(empty($ppp['pppresp1arqid'])){?>
    	$("#pppresp1arqid").parent().parent().hide();
	<?php } else{?>
    	$("#pppresp1").parent().parent().hide();
    <?php } ?>

    <?php if($ppp['pppresp0'] == 'A'){?>
    	$("#pppresp1arqid").parent().parent().show();
	<?php }?>
    
    $("#pppresp0-A").click(function(){
		$("#pppresp1arqid").parent().parent().show();
        $("#pppresp1").parent().parent().hide();
	});

	$("#pppresp2").change(function(){
      	if (this.value == 4) {
           	$("#pppresp2outros").parent().parent().show();
		} else {
			$("#pppresp2outros").parent().parent().hide();
		}
	});

    $("#pppresp3").change(function(){
      	if (this.value == 5) {
			$("#pppresp3outros").parent().parent().show();
		} else {
			$("#pppresp3outros").parent().parent().hide();
		}
	});

    $("#pppresp0-I").click(function(){
       	$("#pppresp1").parent().parent().show();
        $("#pppresp1arqid").parent().parent().hide();
	});
});
</script>