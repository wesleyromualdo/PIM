<?php
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

if (is_array($perfis) && (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis))) {
	$mostrar = 1;
	if ($objPessoaFisica->entcpf == $_SESSION['usucpf']) {
		$mostrar = 1;
	} else {
		$mostrar = 0;
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
                        <li class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Orientações</a>
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
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&prgid=1&inuid=<?php echo $_GET['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>">
		                                        <button class="btn btn-primary">Aceito</button>
		                                    </a>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&prgid=1&inuid=<?php echo $_GET['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>">
		                                        <button class="btn btn-danger">Não Aceito</button>
		                                    </a>
		                                <?php } ?>
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid']; ?>" class="btn btn-primary">Sair</a>
		                            </div>

                        		<?php } elseif ($aceite['adpresposta'] == "S" && !$termoincompleto) { ?>
                        			<div style="margin-top: 5px;" class="text-right">
										<label class="label label-primary">
											Assinado por <?php echo formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
										</label>
									</div>
			                        <div style="text-align: center;margin-top: 20px;">
			                           <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&itrid=<?php echo $itrid; ?>&imprimir=1&inuid=<?php echo $_GET['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>" target="_blanck">
			                               <button class="btn btn-primary imprimir">Imprimir</button>
			                           </a>
			                           <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid']; ?>" class="btn btn-primary">Sair</a>
			                        </div>

                    			<?php } else if (!$termoincompleto) { ?>

		                            <div style="text-align: center;margin-top: 20px;">
		                                <?php if ($mostrar) { ?>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&prgid=1&inuid=<?php echo $_GET['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>&adpid=<?php echo $aceite['adpid']; ?>">
		                                        <button class="btn btn-primary">Aceito</button>
		                                    </a>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&prgid=1&inuid=<?php echo $_GET['inuid']; ?>&prgid=<?php echo $_GET['prgid']; ?>">
		                                        <button class="btn btn-danger">Não Aceito</button>
		                                    </a>
		                                <? } ?>
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid']; ?>" class="btn btn-primary">Sair</a>
		                            </div>

                        		<?php } ?>
                			</div>
                        </div>
	                    <div id="tab-2" class="tab-pane">
							<div class="panel-body">
	                        	<?php echo $proadesaoDados['pfasobre']; ?>
							</div>
	                    </div>
                    </div>
				</div>
			</div>
		</div>
	</div>
</div>