<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "par3/classes/model/seguranca/Usuario.class.inc";
include_once APPRAIZ . "par3/classes/controller/adesao_programa/NovoEnsinoMedio.class.inc";
include_once APPRAIZ . "par3/classes/model/adesao_programa/EscolasNovoEnsinoMedio.class.inc";

$perfis = pegaPerfilGeral($_SESSION['usucpf']);
$prgid = $_REQUEST['prgid'];
$inuid = $_REQUEST['inuid'];
$pfaid = 13; // novo ensino medio
$_SESSION['par3']['adpano_ciclo'] = $adpano_ciclo = $_REQUEST['adpano_ciclo'] ? $_REQUEST['adpano_ciclo'] : ($_SESSION['par3']['adpano_ciclo'] ? $_SESSION['par3']['adpano_ciclo'] : '2018');
$result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid, $adpano_ciclo);

// Instancia objetos
$objOrientacao              = new Par3_Model_Orientacoesprograma();
$objUsuario                 = new Seguranca_Model_Seguranca_Usuario();

if (
    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)
) {
    $insereDoc = 1;
} else {
    $insereDoc = 0;
}

if ((!$result['docid']) && ($insereDoc)) {
    //insere
    
    $docidInserido = wf_cadastrarDocumento(WF_TPDID_NOVO_ENSINO_MEDIO, 'Documento de Adesão Novo Ensino Médio');

    if ($docidInserido) {
        $dadosInicio['adpano'] = date('Y');
        $dadosInicio['adpano_ciclo'] = $adpano_ciclo;
        $dadosInicio['inuid'] = $inuid;
        $dadosInicio['adpid'] = $result['adpid'] != 'null' ? $result['adpid'] : null;
        $dadosInicio['pfaid'] = $pfaid;
        $dadosInicio['docid'] = $docidInserido;
        $dadosInicio['usucpf'] = $_SESSION['usucpf'];
        $dadosInicio['tapid'] = 1;
    }
    
    $modelAdesaoPrograma->popularDadosObjeto($dadosInicio);
    $modelAdesaoPrograma->salvar();
    $modelAdesaoPrograma->commit();
} else {
    $docidInserido = $result['docid'];
}


$result2 = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid, $adpano_ciclo);
$adpid = $result2['adpid'];

$controllerNovoEnsinoMedio = new Par3_Controller_NovoEnsinoMedio();
$controllerNovoEnsinoMedio->validaSeExisteAdesao($adpano_ciclo);

if (!empty($_GET['inuid'])) {
    $aceite = $modelAdesaoPrograma->recuperaPorInuid($_GET['inuid'], $pfaid, $adpano_ciclo);
} else {
    $termoincompleto = true;
}

$_REQUEST['pfaid'] = $pfaid;
$objProgramaAdesao = new Par3_Controller_ProgramaAdesao();
$modelProgramaAdesao = $objProgramaAdesao->recuperar(array());
$_SESSION['par3']['novo_ensino_medio']['vigencia'] = array();
$_SESSION['par3']['novo_ensino_medio']['vigencia']['2018'] = $modelProgramaAdesao->pfadatafinal;

$arquivoResolucao = $modelProgramaAdesao->arqid;

if($_GET['arquivo_resolucao'])
{
    $file = new FilesSimec();
    $file-> getDownloadArquivo($_GET['arquivo_resolucao']);
    exit;
}

$restricaoMinimaGrupo1 = false;
//Sobre quantidade escolas já salvas
$dadosSobreEscola['inuid']  = $inuid;
$dadosSobreEscola['adpid']  = $adpid;
$escolasSelecionadas        = $controllerNovoEnsinoMedio->buscaQTDEscolasSelecionadas($dadosSobreEscola);

$arrRestricoes = array();

if ($aceite['adpresposta'] != 'S') {
    $arrRestricoes[]['dsc'] = '<span style="color:red" > Termo não aceito </span>';
}
if ($restricaoMinimaGrupo1) {
    $arrRestricoes[]['dsc'] = '<span style="color:red"> Selecione no mínimo uma escola</span>';
}

if ((!$docidInserido) && ($insereDoc)) {
    ?>
    Faltam dados. <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
    <?php
    die("");
}

$resposta = $aceite['adpresposta'];
$editavel = false;
if ($resposta == "S") {
    $editavel = true;
}

$disabled = ($editavel) ? '' : 'disabled=disabled';
$situacaoAtual = wf_pegarEstadoAtual($docidInserido);
$esdid = $situacaoAtual['esdid'];

if ($aceite['adpresposta'] == 'S' && $esdid == WF_ESDID_NAO_INICIADO_PNEM) {
    if ($esdid == WF_ESDID_NAO_INICIADO_PNEM || !$esdid) {
        wf_alterarEstado($docidInserido, WF_AEDID_PNEM_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
        $situacaoAtual = wf_pegarEstadoAtual($docidInserido);
        $esdid = $situacaoAtual['esdid'];
    }
}

$bloqueiaGeral = FALSE;
$bloqueiaValidade = FALSE;
$vencido = false;

if( (strtotime($_SESSION['par3']['novo_ensino_medio']['vigencia'][$adpano_ciclo] . ' 23:59:00') < strtotime(date('Y-m-d G:i:s')))
    ){
    $bloqueiaGeral = TRUE;
    $vencido = true;
}

if (
    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)
) {
    $mostrar = true;
} else {
    $mostrar = false;
}

if ($itrid == 2) {
    // municipal
    $where[] = '1=0';
} else {
    // estadual
    $where[] = 't1.tp_dependencia = 2';
    $where[] = "t1.estuf = '{$modelInstrumentoUnidade->estuf}'";
}

if ($itrid == 2) {
} else {
    $proadesaoDados['pfatermoestado'] = str_replace('\"', '"', $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] = str_replace('#dirigente#', $objPessoaFisica->entnome, $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] = str_replace('#dirigente_cpf#', formatar_cpf($objPessoaFisica->entcpf), $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] = str_replace('#nome_entidade#', $inunome, $proadesaoDados['pfatermoestado']);
    $proadesaoDados['pfatermoestado'] = str_replace('#data_atual#', date('d/m/Y'), $proadesaoDados['pfatermoestado']);
    $termoPreenchido = $proadesaoDados['pfatermoestado'];
}

switch ($_REQUEST['requisicao']) {
    case 'enviarSelecao':
        $_REQUEST['docid'] = $docidInserido;
        $controllerNovoEnsinoMedio->enviarSelecao($_REQUEST);
        break;
    case 'aba_sintese':
        ob_clean();
        require_once APPRAIZ."par3/modulos/principal/adesao/termos/novo_ensino_medio/aba_sintese.php";
        die();
        break;
    case 'pdf_sintese':
        ob_clean();
        ?>
        <div id="conteudo_sintese">
        <center>
        <h3>Sintese de escolas que participarão do Programa Novo Ensino Médio</h3>
        </center>
        <hr class="divider">
        <center>
        <h3>Escolas Sorteadas</h3>
        </center>
        <?php
        $dados = array(
            'adpid'          => $adpid,
            'situacao'       => 1,
            'somenteLeitura' => true,
            'inuid'          => $inuid
        );
        $controllerNovoEnsinoMedio->listaEscolas($dados);
        ?>
            <hr class="divider">
            <center>
                <h3>Escolas Selecionadas</h3>
            </center>
    		<?php 
			    $dados = array(
			        'adpid'          => $adpid,
			        'situacao'       => 2,
			        'somenteLeitura' => true,
			        'inuid'          => $inuid,
			        'selecionadas'   => 'S'
			    );
			    $controllerNovoEnsinoMedio->listaEscolas($dados); 
		    ?>
        </div>
        <?php 
        die();
        break;
    case 'listarEscolas':
        ob_clean();
        $controllerNovoEnsinoMedio->listaEscolas($_REQUEST);
        die();
        break;
    case 'vincularEscola':
        ob_clean();
        $controllerNovoEnsinoMedio->vincularEscolas($_REQUEST);
        die();
        break;
    case 'pdf_sintese':
        ob_end_clean();
        $valor = $_REQUEST['conteudo_pdf'];
        $conteudo = str_replace("\\", " ", $valor);

        html2Pdf("
		
				<style>
				.table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
				border: 1px solid #e7e7e7;
		}
				.table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
				background-color: #f5f5f6;
				border-bottom-width: 1px;
		}
				.table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
				border-bottom-width: 2px;
		}
				.table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
				border: 1px solid #ddd;
		}
				.table-condensed > thead > tr > th, .table-condensed > tbody > tr > th, .table-condensed > tfoot > tr > th, .table-condensed > thead > tr > td, .table-condensed > tbody > tr > td, .table-condensed > tfoot > tr > td {
				padding: 5px;
		}
				.table > thead > tr > th {
				border-bottom: 2px solid #ddd;
				vertical-align: bottom;
		}
				.table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td {
				border-top: 1px solid #ddd;
				line-height: 1.42857;
				padding: 8px;
				vertical-align: top;
		}
				td, th {
				padding: 0;
		}
				th {
				text-align: left;
		}
                    td, th {
                    padding: 0;
		}
                * {
                    box-sizing: border-box;
		}
                    *::-moz-placeholder {
                    color: #d1d1d1;
		}
                    *::-moz-placeholder {
                    color: #d1d1d1;
		}
                    .alert-info {
                    background-color: #d9edf7;
                    border-color: #bce8f1;
                    color: #31708f;
		}
                    .alert {
                    border: 1px solid transparent;
                    border-radius: 4px;
                    margin-bottom: 20px;
                    padding: 15px;
		}
                    h3, h4, h5 {
                    font-weight: 600;
                    margin-top: 5px;
		}
                    h3 {
                        font-size: 16px;
                    }
                    h1, h2, h3, h4, h5, h6 {
                    font-weight: 100;
		}
                    h3 {
                    font-size: 2.92rem;
                    line-height: 110%;
                    margin: 1.46rem 0 1.168rem;
		}
                    .table {
                    margin-bottom: 20px;
                    max-width: 100%;
                    width: 100%;
		}
                    .table-bordered {
                    border: 1px solid #ebebeb;
		}
                    table {
                    border-collapse: collapse;
                    border-spacing: 0;
		}
                    table {
                    background-color: transparent;
		}
                    .header {
                    background: #000;
                    margin: 0 auto;
                    width: 100%;
                    padding-top: 2em;
                    padding-bottom: 2em;
                    position: relative;
                    overflow: hidden;
                }
		
                </style>
                <div id=\"conteudo_sintese\">
                {$conteudo}				 
		</div>");

        break;
    case 'pdf_termo':

        ob_end_clean();

        $mensagemAssinatura = "";
        if ($aceite['adpresposta'] == "S") {
            $nomeA = $objUsuario->getNome($aceite['usucpf']);
            $cpfA = formatar_cpf($aceite['usucpf']);
            $horaA = formata_data_hora($aceite['adpdataresposta']);

            $mensagemAssinatura = '<span style="color:green; font-size:10px;">' . "Assinado por {$nomeA} CPF {$cpfA}. No dia {$horaA} </span> ";
        }

        $valor = $termoPreenchido;
        $conteudo = str_replace("\\", " ", $valor);
        $conteudo = str_replace("a ser denominado", "a ser denominado<br><br><br>", $conteudo);

        $nome = $objUsuario->getNome($_SESSION['usucpf']);

        $rodape = '<span style="color:red; font-size:10px;"> Impresso por ' . $nome . ' CPF nº ' . formatar_cpf($_SESSION['usucpf']) . ' No dia ' . date('d/m/Y') . ' as ' . date('H:i:s') . ' dentro do módulo de adesões do SIMEC</span>';

        ob_clean();
        
        $nome = $objUsuario->getNome($aceite['usucpf']);
        
        $exibeComeco = '';
        if ($nome != '') {
            $exibeComeco = $nome . ' CPF: ';
            
            echo "
    				<html>
    				  <head>
    				    <style>
    				    
    				      .footer { position: fixed; bottom: 0px; }
    				    </style>
    				  </head>
    				  <body>
    				    {$conteudo}
                        <div style=\"margin-top: 5px;text-align: right;\" >";
                
            if (strftime("%H:%M:%S", strtotime($aceite['adpdataresposta'])) != '00:00:00') { 
                echo "Assinado por $exibeComeco".formatar_cpf($aceite['usucpf'])." em ".formata_data_hora($aceite['adpdataresposta']).".";
            } else {
                echo "Assinado por $exibeComeco ".formatar_cpf($aceite['usucpf'])." em ".formata_data($aceite['adpdataresposta']).".";
            } 
            echo "      </div>  
    				  </body>
    				</html>";
        }
	    die();
	    break;
    case 'salvar_orientacao':
        
        $orpidSave = $_POST['orpid'];
        $orptextoSave = $_POST['orptexto'];
        
        switch ($orpidSave) {
            case ORIENTACOES_NEM_ABA_ESCOLAS:
                $abaAtual = '2';
                break;
        }
        
        $dadosOrientacao = array();
        
        if ($orptextoSave) {
            $dadosOrientacao['orpid']       = $orpidSave;
            $dadosOrientacao['orptexto']    = $orptextoSave;
            $dadosOrientacao['prgid']       = PRGID_NOVO_ENSINO_MEDIO;
            
            $objOrientacao->popularDadosObjeto($dadosOrientacao);
            
            $orpidSave = $objOrientacao->salvar();
            if ($objOrientacao->commit()) {
                $nomeAba = $objOrientacao->getOrpDescricaoByID($orpidSave);
                $msgAlert = $nomeAba . " salvas com sucesso";
                simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid=' . $inuid . "&prgid=" . $prgid . "&abaatual={$abaAtual}", 'success', $msgAlert);
                die();
            }
            
        }
        break;
    default:
        break;
}

if ($_GET['aceite'] == 'S') {

    $dados['adpano_ciclo'] = $_GET['adpano_ciclo'];

    $dados['inuid'] = $_GET['inuid'];
    $dados['adpid'] = $_GET['adpid'];
    $dados['prgid'] = $_GET['prgid'];
    $dados["pfaid"] = $pfaid;

    if (empty($pfaid) && !empty($_GET['pfaid'])) {
        $dados['pfaid'] = $_GET['pfaid'];
    }

    $dados['adpresposta'] = $_GET['aceite'];
    $dados['tapid'] = 1;

    if (empty($proadesaoDados['pfaano'])) {
        $dados['adpano'] = date('Y');
    } else {
        $dados['adpano'] = $proadesaoDados['pfaano'];
    }

    $dados['usucpf'] = $_SESSION['usucpf'];
    $dados['adpdataresposta'] = "'now()'";

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
        if ($docidInserido) {
            if (($esdid == WF_ESDID_NAO_INICIADO_PNEM) || (!$esdid)) {
                wf_alterarEstado($docidInserido, WF_AEDID_PNEM_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
            }
        }
        $msgAlert = $proadesaoDados['pfamsgdeaceitacao'];
    } else {
        $msgAlert = $proadesaoDados['pfamsgdenaoaceitacao'];
    }

    $_SESSION[$_POST['pppid']]['termoaceito'] = true;

    simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid=' . $dados['inuid'] . "&prgid=" . $dados['prgid'], 'success', $msgAlert);
}