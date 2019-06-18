<?php
//error_reporting(E_ALL);
include_once APPRAIZ . 'par3/classes/helper/ProgramaFPMHelper.class.inc';
include_once APPRAIZ . 'par3/classes/controller/ProgramaFPMMunicipio.class.inc';
include_once APPRAIZ . 'par3/classes/model/seguranca/Usuario.class.inc';
include_once APPRAIZ . 'www/par3/_funcoes.php';
$objUsuario = new Seguranca_Model_Seguranca_Usuario();
$helper = new Par3_Helper_ProgramaFPMHelper();
$fpmController = new Par3_Controller_ProgramaFPMMunicipio();
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

if( !in_array(PAR3_PERFIL_SUPER_USUARIO,$perfis) &&
    !in_array(PAR3_PERFIL_ADMINISTRADOR,$perfis) &&
    !in_array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR,$perfis)){
    header("Location:par3.php?modulo=principal/listaMunicipios&acao=A");
}



//carrega os dados da tela corretamente
$pfasobre      = "";
$dadosPrograma = "";
$dadosDaAdesao = null;
$dadosCamposMatriculas = array(
        'fpmvltotalmatriculaatendidapddeintegral' => 357,

);
$dadosProgramaAdesao = array();
$dadosEscolasDaUnidade = array();
$dadosProgramaMunicipio = array();
$dataTermino = "";
$dataTerminoFormato = "";
$fimDoPrograma = false;
$dadosDaUnidade = array();
$imprimirEstouDeAcordo = false;
$dadosUsuarioAssinou = array();


if(isset($_REQUEST['requisicao'])){
    //caso o usuário esteja querendo alterar o campo Orientações Gerais
    if($_REQUEST['requisicao'] == 'salva_orientacao_geral'){
        if(!empty($_POST['prgid']) && !empty($_POST['pfasobre'])){
            $result = $fpmController->updateSobre($_POST);
            if($result){
                $docidInserido = wf_cadastrarDocumento(WF_TPDID_FPM, 'Cadastramento iniciado', PAR3_FLUXO_FPM_EM_CADASTRAMENTO);
                $fpmController->updateEstadoDocumento($_REQUEST['inuid'],$docidInserido);
                //verifica se unidade está no estado
            }
            if($result){
                $helper->setOrientacoesGerais($_POST['pfasobre']);
            }
        }
    }

    if($_REQUEST['requisicao'] == "salva_por_escola"){
        $inuid = $_GET['inuid'];

        $validaDados = $fpmController->validaValorMunicipioEscolas($_REQUEST);

       if($validaDados['erro'] == true){
            $helper->getResulValidation($validaDados);
        }else{
           //se for preenchido e enviado valores por escola
                   if(is_array($_POST['vlestudanteporescola']) && count($_POST['vlestudanteporescola']) > 0){
                       $dadosMunicipioJaCadastrado = $fpmController->getDadosMunicipioByUnidade($_GET['inuid']);
                       if(!empty($dadosMunicipioJaCadastrado['fmpid'])){
                           $_REQUEST['fmpid'] = $dadosMunicipioJaCadastrado['fmpid'];
                       }
                       $salvouDadosMunicipio = $fpmController->salvaDadosMunicipio($_REQUEST);
                       if($salvouDadosMunicipio){
                           $salvo = $fpmController->salvaDadosPorEscola($_REQUEST);
                           ///$docidInserido = wf_cadastrarDocumento(WF_TPDID_FPM, 'Cadastramento iniciado', PAR3_FLUXO_FPM_EM_CADASTRAMENTO);
                           if($salvo){
                               $dadosWorkFlow = $fpmController->getDadosWorkFlowUnidadePrograma($_REQUEST['inuid'],11);
                               //ver($dadosWorkFlow);
                               if($dadosWorkFlow['esdid'] == PAR3_FLUXO_FPM_REABERTO){
                                   wf_alterarEstado($dadosWorkFlow['docid'],WF_AEDID_FPM_REABERTO_EMCADASTRAMENTO,"O Secretário alterou os valores após ter reaberto.",array(),array());
                                   echo "<script> $('#aba-plano').trigger('click'); </script>";
                               }
                           }
                       }
                   }
           }
    }

    if($_POST['requisicao'] == "tramita_reabrir"){
        ob_clean();
        if(!empty($_POST['docid'])){
            $tramitou = wf_alterarEstado($_POST['docid'],WF_AEDID_FPM_EMCADSTRAMENTO_REABRIR,"Secretário reabriu a adesão.",array(),array());
            if($tramitou == true){
                echo "T";
            }else{
                echo "F";
            }
            exit();
        }else{
            echo "F";
            exit();
        }
        die();
    }

    if($_POST['requisicao'] == "tramita_enviadofnde"){
        ob_clean();
        if(!empty($_POST['docid'])){
            $tramitou = wf_alterarEstado($_POST['docid'],WF_AEDID_FPM_EMCADSTRAMENTO_ENVIADOAOMEC,"Secretário enviou a adesão ao FNDE.",array(),array());
            if($tramitou == true){
                echo "T";
            }else{
                echo "F";
            }
            exit();
        }else{
            echo "F";
            exit();
        }
        die();
    }

}


if(!empty($_GET['prgid']) && !empty($_GET['inuid'])){
    $dadosProgramaMunicipio = $fpmController->getDadosMunicipioByUnidade($_GET['inuid']);
    $dadosDaUnidade = $fpmController->getDadosDaUnidade($_GET['inuid']);
    $dadosProAdesao = $modelProadesao->pegaLinha("select * from par3.proadesao where prgid = 54");
    $dadosUsuarioAssinou = $modelAdesaoPrograma->getDadosDoUsuarioAssinou($_GET['inuid']);
    //pega o valor do campo orientações gerais
    $dadosPrograma = $fpmController->fetchAllProgramaById($_GET['prgid']);
    $dataTermino = (!empty($dadosPrograma['prgperiodofim'])) ? $dadosPrograma['prgperiodofim'] : null;
    //$periodoValido = ($dataTermino >= date("Y-m-d")) ? true : false;
    $periodoValido = ($dataTermino <= date("Y-m-d")) ? false : true;
    $date = new DateTime($dadosPrograma['prgperiodofim']);//pega data para formatar
    $dataTerminoFormato = $date->format("d-m-Y");
    $helper->setNoPeriodo($periodoValido);
    $pfasobre = $dadosPrograma['pfasobre'];
    $dadosProgramaAdesao = $modelAdesaoPrograma->pegaLinha("select * from par3.prodesaoprograma where inuid = {$_GET['inuid']} and pfaid = 11");
    //pega o valor dos campos
    //Total de Matrículas de 3º ao 9ª ano da Rede
    //Valor a ser aplicado no PNME pelo município se houver
    //Porém verifica se não já está cadastrado o valor na tabela do programa
    $dadosUnidadeNoPrograma = $fpmController->getProgramaFPMByIdUnidade($_GET['inuid']);

    //Dados do WorkFlow
    $dadosEstadoDocumento = $fpmController->getDadosWorkFlowUnidadePrograma($_GET['inuid'],$dadosProAdesao['pfaid']);

    //Se o municipio estiver em cadastramento
    if($dadosEstadoDocumento['esdid'] == PAR3_FLUXO_FPM_EM_CADASTRAMENTO){
        $helper->setBtnSalvarFormulario($dadosEstadoDocumento['esdid']);
        $helper->setDocId($dadosEstadoDocumento['docid']);
        $helper->setEstadoDocumento($dadosEstadoDocumento['esdid']);
        $dadosCamposTotalMatriculas = $fpmController->carregaValueCamposMatriculas($_GET['inuid']);
        $helper->setTotalMatriculaEnsinoFundamental($dadosCamposTotalMatriculas['total_matricula_ensino_fundamental']);
        $helper->setTotalMatriculasAtendidasPddeIntegral($dadosCamposTotalMatriculas['total_matriculas_atendidas_pdde_integral']);
        $helper->setTotalMatriculaTempoIntegralCenso($dadosCamposTotalMatriculas['total_matricula_tempo_integral_censo']);
        $dadosFndeDoMunicipio = $fpmController->getDadosFnde2018ByUnidade($_GET['inuid']);
        $helper->setVlRepassadoResolucaoFnde2018($dadosFndeDoMunicipio['vl_repassefnde2018']);
        $helper->setVlAplicadoPnmeMunicipio($dadosCamposTotalMatriculas['fpmvaloraplicadopnmemunicipio']);
        $helper->setDataLimitePrograma($dadosPrograma['pfadatafinal']);
        $dadosEscolasDaUnidade = $fpmController->cerregaValoresPorEscola($_GET['inuid']);
        $dadosEscolasDaUnidadeJaComValores = $fpmController->cerregaValoresPorEscolaJaComValores($_GET['inuid']);
        //Se a escola já tem dados salvos, bloqueia
        if($dadosEscolasDaUnidadeJaComValores['qt'] > 0){
            $helper->setVisibilidadeForm(false);
        }else{
            $helper->setVisibilidadeForm(true);
        }
        $helper->setListaEscolasDaUnidade($dadosEscolasDaUnidade);
        $dadosDaAdesao  = $fpmController->getDadosDaAdesaoByIdInstrumentoUnidade($_GET['inuid']);
        $dadosCarga     = $fpmController->getDadosCargaProgramaNovoMaisEducacao($_GET['inuid']);
    }


    if($dadosEstadoDocumento['esdid'] == PAR3_FLUXO_FPM_REABERTO){
        $helper->setBtnSalvarFormulario($dadosEstadoDocumento['esdid']);
        $helper->setDocId($dadosEstadoDocumento['docid']);
        $helper->setEstadoDocumento($dadosEstadoDocumento['esdid']);
        $dadosCamposTotalMatriculas = $fpmController->carregaValueCamposMatriculas($_GET['inuid']);
        $helper->setTotalMatriculaEnsinoFundamental($dadosCamposTotalMatriculas['total_matricula_ensino_fundamental']);
        $helper->setTotalMatriculasAtendidasPddeIntegral($dadosCamposTotalMatriculas['total_matriculas_atendidas_pdde_integral']);
        $helper->setTotalMatriculaTempoIntegralCenso($dadosCamposTotalMatriculas['total_matricula_tempo_integral_censo']);
        $dadosFndeDoMunicipio = $fpmController->getDadosFnde2018ByUnidade($_GET['inuid']);
        $helper->setVlRepassadoResolucaoFnde2018($dadosFndeDoMunicipio['vl_repassefnde2018']);
        $helper->setVlAplicadoPnmeMunicipio($dadosCamposTotalMatriculas['fpmvaloraplicadopnmemunicipio']);
        $helper->setDataLimitePrograma($dadosPrograma['pfadatafinal']);
        $dadosEscolasDaUnidade = $fpmController->cerregaValoresPorEscola($_GET['inuid']);
        $dadosEscolasDaUnidadeJaComValores = $fpmController->cerregaValoresPorEscolaJaComValores($_GET['inuid']);
        $helper->setVisibilidadeForm(true);
        $helper->setListaEscolasDaUnidade($dadosEscolasDaUnidade);
        $dadosDaAdesao  = $fpmController->getDadosDaAdesaoByIdInstrumentoUnidade($_GET['inuid']);
        $dadosCarga     = $fpmController->getDadosCargaProgramaNovoMaisEducacao($_GET['inuid']);
    }

    if($dadosEstadoDocumento['esdid'] == PAR3_FLUXO_FPM_NAO_ACEITOU_TERMO){
        $imprimirEstouDeAcordo = true;
    }
}



/*Carrega as trancas da página*/
//Se o perfil logado for Super usuário ou Administrador
if (in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO_DESENVOLVEDOR, $perfis)
) {
    $helper->setPerfilPodeEditar(true); //perfil tem permissão de alterar
	$mostrar = 0;
} else {
    $helper->setPerfilPodeEditar(false); //perfil não tem permissão para alterar
	$mostrar = 0;
}

if ($itrid == 2) {//municipal
	$where[] = 't1.tp_dependencia = 3';
	$where[] = "t1.estuf = '{$modelInstrumentoUnidade->estuf}'";
	$where[] = "t1.muncod = '{$modelInstrumentoUnidade->muncod}'";
} else {//estadual
	$where[] = 't1.tp_dependencia = 2';
	$where[] = "t1.estuf = '{$modelInstrumentoUnidade->estuf}'";
}

$escolasPrioritariasLista = $modalEscolasPrioritarias->recuperarTodos('t1.pepescola, t1.estuf, t1.co_entidade, t2.mundescricao', $where, null, array('join' => 'muncod'));

$escolasPrioritarias =
'<table class="table">
    <thead>
        <th>Escola</th>
		<th>Código INEP</th>
		<th>Estado</th>
        <th>Município</th>
    </thead>
<tbody>';

foreach($escolasPrioritariasLista as $key) {
	$escolasPrioritarias .=
	"<tr>
		<td>{$key['pepescola']}</td>
		<td>{$key['co_entidade']}</td>
		<td>{$key['estuf']}</td>
		<td>{$key['mundescricao']}</td>
	</tr>";
}

$escolasPrioritarias .=
	'</tbody>
</table>';

if ($itrid == 2)
{
    $proadesaoDados['pfatermomunicipio'] =  str_replace('\"','"', $proadesaoDados['pfatermomunicipio']);
    $proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermomunicipio']);
    $proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermomunicipio']);
    $proadesaoDados['pfatermomunicipio'] =  str_replace('#nome_entidade#', $inunome, $proadesaoDados['pfatermomunicipio']);
    $proadesaoDados['pfatermomunicipio'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermomunicipio']);
    $proadesaoDados['pfatermomunicipio'] =  str_replace('#escolas_prioritarias#', $escolasPrioritarias, $proadesaoDados['pfatermomunicipio']);

    $termoPreenchido = $proadesaoDados['pfatermomunicipio'];
}
else
{
    $proadesaoDados['pfatermoestado'] =  str_replace('\"','"', $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] =  str_replace('#nome_entidade#',$inunome, $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] =  str_replace('#escolas_prioritarias#', $escolasPrioritarias, $proadesaoDados['pfatermoestado']);

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

    $arrEscola = array();

    foreach($escolasPrioritariasLista as $escola){
        $arrEscola[] = $escola['co_entidade'];
    }

    $sql = "UPDATE maiseducacao2016.maiseducacao SET
                maeprioritariaaderiu = true
            WHERE
                pdicodinep IN ('".implode("','", $arrEscola)."');";


    adapterConnection::pddeinterativo()->executar($sql);
    adapterConnection::pddeinterativo()->commit();

    if ($_GET['aceite'] == 'N') {
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid='.$dados['inuid'], 'success', $msgAlert);
    }
    else {
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$dados['inuid']."&prgid=".$dados['prgid'], 'success', $msgAlert);
    }
}
?>


<!--
<div style="text-align: center;margin-top: 20px;">
    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&requisicao=imprimir_termo_fpm&inuid=<?php /*echo $_GET['inuid'] */?>&prgid=<?php /*echo $_GET['prgid'] */?>"
       target="_blanck">
        <button class="btn btn-primary imprimir">Imprimir</button>
    </a>
    <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php /*echo $_GET['inuid'] */?>" class="btn btn-primary">Sair</a>
</div>
-->

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
                        <?php if($aceite['adpresposta'] == "S"): ?>
                        <li class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false" id="aba-plano">Plano do Município</a>
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
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>">
		                                        <button class="btn btn-primary">Aceito e estou de acordo</button>
		                                    </a>
                                            <?php if($aceite['adpresposta'] != "N"):?>
		                                        <button class="btn btn-danger" onclick="js_cetezaNaoAceita();">Não estou de acordo</button>
                                            <?php endif;?>
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
                                        <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&requisicao=imprimir_termo_fpm&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>"
                                           target="_blanck">
                                            <button class="btn btn-primary imprimir">Imprimir</button>
                                        </a>
			                           <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
			                        </div>
                    			<?php } elseif ($aceite['adpresposta'] == "N"){ ?>
                                    <div style="margin-top: 5px;" class="text-right">
                                        <label class="label label-danger">
                                            Termo negado por <?php echo $dadosUsuarioAssinou['usunome']." - ".$dadosUsuarioAssinou['usucpf'];?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
                                        </label>
                                    </div>
                                    <div style="text-align: center;margin-top: 20px;">
                                        <?php if($imprimirEstouDeAcordo == true && $periodoValido == true): ?>
                                            <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpid=<?php echo $aceite['adpid'] ?>">
                                                <button class="btn btn-primary">Aceito e estou de acordo</button>
                                            </a>
                                        <?php if($aceite['adpresposta'] != "N"):?>
                                            <button class="btn btn-danger" onclick="js_cetezaNaoAceita();">Não estou de acordo</button>
                                            <?php endif;?>
                                            <a onclick="js_ImprimirNegado();" class="btn btn-primary">Imprimir</a>
                                        <?php endif;?>
                                    </div>
                                <?php }

                    			else if (!$termoincompleto) { ?>

		                            <div style="text-align: center;margin-top: 20px;">
		                                <?php
                                        $mostrar = ($dadosProgramaAdesao['adpresposta'] == "N") ? false : "true";
                                        if ($periodoValido == true) { ?>
		                                    <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpid=<?php echo $aceite['adpid'] ?>">
		                                        <button class="btn btn-primary">Aceito e estou de acordo</button>
		                                    </a>
                                            <?php if($aceite['adpresposta'] != "N"):?>
                                            <button class="btn btn-danger" onclick="js_cetezaNaoAceita();">Não estou de acordo</button>
                                            <?php endif;?>
		                                <? } ?>
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
                                        <?php if($imprimirEstouDeAcordo == true && $periodoValido == true): ?>
                                            <a onclick="js_ImprimirNegado();" class="btn btn-primary">Imprimir</a>
                                        <?php endif;?>
		                            </div>

                        		<?php } ?>
                			</div>
                        </div>
	                    <?php if($aceite['adpresposta'] == "S"): ?>
                        <div id="tab-2" class="tab-pane" class="t">
                            <?php
                                $helper->setOrientacoesGerais($pfasobre);
                                $helper->includeSubView("plano_do_municipio_aba");
                            ?>
	                    </div>
                        <?php endif; ?>
                    </div>
				</div>
			</div>
		</div>
	</div>
</div>






<script>


    function js_cetezaNaoAceita(){
        swal({
            title: "<span style='font-size: 27px'>Atenção</span>",
            text: "Tem certeza que deseja continuar?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                var url =  window.location.href;
                var unidade = "<?php echo $_GET['inuid'];?>";
                var prgid = "<?php echo $_GET['prgid'];?>";
                var requisicao = "nao_aceita";
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "&inuid="+unidade+"&requisicao="+requisicao+"&prgid="+prgid+"&aceite=N",
                    success: function(returno) {
                        if(returno == "T"){
                            uri = url.split("#");
                            if(uri[0]){
                                js_naoAceitouMensagem(uri[0]);
                            }else{
                                alert("Não foi possível concluir, por favor tente mais tarde!");
                            }
                        }
                    }
                });
            } else {
                return false;
            }
        });
    }

    function js_naoAceitouMensagem(url){
        var municipio = '<?php echo $dadosDaUnidade['mundescricao'];?>';
        var dataLimite = '<?php echo $dataTerminoFormato; ?>';
        swal({
            title: "<span style='font-size: 27px'>Atenção</span>",
            text: "Operação realizada com sucesso. O município "+municipio+" ainda poderá mudar de opção e aderir ao Programa até o fechamento do período de adesão "+dataLimite,
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Ok",
            closeOnConfirm: "on",
            //cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                window.location = url;
            } else {
                return false;
            }
        });
    }

    function js_desabilitaAba(aba){
        switch(aba){
            case 'PLANO_MUNICIPIO':
                    $(".t").hide();
                break;
        }
    }

    function js_imprimiTermoPdf(){
        var requisicao = "imprimir_termo_fpm";
        var unidade    = "<?php echo $_GET['inuid']?>";
        var prgid    = "<?php echo $_GET['prgid']?>";
        $.ajax({
            type: "GET",
            url: window.location.href,
            data: "&inuid="+unidade+"&requisicao="+requisicao+"&prgid="+prgid,
            success: function(returno) {

            }
        });
    }

    </script>