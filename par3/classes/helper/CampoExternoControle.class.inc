<?php
class Par3_Helper_CampoExterno {
	private $htm;
	private static $camposDefault;

	public function montaNovoCampo( $perid, $qrpid, $percent = 90, $habilita = true ){

		$obQestionario = new QQuestionarioResposta();

		$queid = $obQestionario->pegaQuestionario( $qrpid );

		$obQuestAnexo = new Par3_Model_QuestoesEstrategicasAnexos( );
		$dadosResp    = $obQuestAnexo->carregarResposta( $qrpid, $perid );

		$pergunta = new QPergunta($perid);
		$itemPaiPergunta = new QItemPergunta($pergunta->itpid);

        #Perid
        $arrEscolasCE                   = array(4602, 4707,   4434, 4345, 4256, 4811);
        $arrGremio                      = array(4605, 4710,   4437, 4348, 4259, 4814);
        $arrPNE                         = array(4410, 4321, 4232);
        $arrPlanoEducacao               = array(4231, 4320, 4409);
        $arrEscolasEPT                  = array(4770, 4866);
        $arrEscolasEstEPT               = array(4771, 4867);
        $arrEscolasProeja               = array(4773, 4868);

        if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
            $arrEstudoDemanda = array(
                645, 655, 718, 730, 680, 698, 542, 552, 615, 627, 577, 595, //Estimativa de demanda de alunos
                646,719,681,616,578,543, //Origem da demanda
                647,720,682,617,579,544, //Modelo de projeto
            );
        }else{
            $arrEstudoDemanda = array();
        }

		$htm = '';
		if (in_array($perid, $arrEstudoDemanda)) {
		    $ctrEstudoDemanda = new Par3_Controller_EstudoDemanda();
		    $htm .= $ctrEstudoDemanda->retornaCampoExterno($perid, $qrpid, $_REQUEST);
		} else if (in_array($perid, $arrEscolasCE)) {
    		$obQuestEscolaCE = new Par3_Model_QuestoesEstrategicasEscolasCe();
    		$qdtEscolas      = $obQuestEscolaCE->retornarQtdEscolas( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas com CE em funcionamento
    		         <i class=\"fa fa-pencil-square-o escolas_ce\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escola da redemunicipal de ensino: <span id=total_escolas_ce >$qdtEscolas</span>";
		} else if (in_array($perid, $arrGremio)) {
    		$obQuestEscolaGremio = new Par3_Model_QuestoesEstrategicasEscolasGremio();
    		$qdtEscolas      = $obQuestEscolaGremio->retornarQtdEscolas( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas com Grêmio em funcionamento
    		         <i class=\"fa fa-pencil-square-o escolas_gremio\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escola da redemunicipal de ensino: <span id=total_escolas_gremio >$qdtEscolas</span>";
		} else if (in_array($perid, $arrPNE)) {
    		$htm .= " <div clas=\"ibox-content\" style=\"text-align:justify;text-justify:inter-word;\">
    		                  O Plano Nacional de Educação (PNE), instituído pela Lei n° 13.005/2014,
    		              determinou, para o primeiro ano de vigência, a elaboração ou adequação dos
    		              planos estaduais, distrital e municipais de educação, em consonância com o
    		              texto nacional. No dia 24 de junho de 2015 venceu o prazo para todas as
    		              prefeituras, estados e o Distrito Federal finalizarem seus planos
    		              de educação, ou adequarem os planos já aprovados em lei às diretrizes
    		              do PNE. Se o seu estado/município ainda não publicou a lei com o plano
    		              de educação, poderá entrar em contato com a SASE (Secretaria de Articulação
    		              com os Sistema de Ensino), pelo e-mail XXXXXXXX, ou telefone(s) XXXXXXXXXX
    		          </div>";
		} else if (in_array($perid, $arrPlanoEducacao)) {
			$htm .= " Arquivo em anexo: <a onclick=\"janela(window.location.href+'&req=download&arqid={$dadosResp['arqid']}', 600, 480);\">
			             {$dadosResp['arqdescricao']}</a><br />";
                    if (!$dadosResp['arqid']) {
                                $htm .= " <div clas=\"ibox-content\" style=\"color: red;text-align:justify;text-justify:inter-word;\">";
                                $htm .= "         Caso o estado/município possua o plano Estadual/Municipal de Educação
                                              com a sua lei devidamente sancionada e o sistema não mostre o arquivo anexado,
                                              deverá entrar em contato com a equipe responsável na SASE(Secretaria de Articulação
                                              com os Sistemas de Ensino), pelo e-mail XX, ou telefone (x) xx";
                                $htm .= " </div>";
                    }
		} else if (in_array($perid, $arrEscolasEPT)) { #Escolas Exclusivas EPT
    		$obQuestEscolaEPT = new Par3_Model_QuestoesEstrategicasEscolasEPT();
    		$qdtEscolas      = $obQuestEscolaEPT->retornarQtdEscolasEPT( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas da rede estadual exclusivas de Educação Profissional e Tecnológica (EPT)
    		         <i class=\"fa fa-pencil-square-o escolas_ept\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escolas da rede estadual exclusivas de Educação Profissional e Tecnológica (EPT):
                         <span id=total_escolas_ept >$qdtEscolas</span>";
		} else if (in_array($perid, $arrEscolasEstEPT)) { #Escolas Estaduais que oferecem EPT
    		$obQuestEscolaEstEPT = new Par3_Model_QuestoesEstrategicasEscolasEstEPT();
    		$qdtEscolas      = $obQuestEscolaEstEPT->retornarQtdEscolasEstEPT( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas da rede estadual que oferecem Educação Profissional e Tecnológica (EPT)
    		         <i class=\"fa fa-pencil-square-o escolas_estept\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escolas da rede estadual que oferecem Educação Profissional e Tecnológica (EPT):
                         <span id=total_escolas_estept >$qdtEscolas</span>";
		} else if (in_array($perid, $arrEscolasProeja)) { #Escolas PROEJA
    		$obQuestEscolaProeja = new Par3_Model_QuestoesEstrategicasEscolasProeja();
    		$qdtEscolas      = $obQuestEscolaProeja->retornarQtdEscolasProeja( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas da rede estadual com PROEJA
    		         <i class=\"fa fa-pencil-square-o escolas_proeja\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escolas da rede estadual com PROEJA:
                         <span id=total_escolas_proeja >$qdtEscolas</span>";
		} else {
    		if( $dadosResp['arqid'] ){
    			if ( $habilita ) {
    				$boExcluir = "<a href=\"#\" onclick=\"excluirArquivo( ".$dadosResp['arqid'].", ".$perid." );\" title=\"Excluir Anexo\">
    				                    <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">
    				                </a>";
    			} else {
    				$boExcluir = "<a href=\"#\" title=\"Excluir Anexo\">
    				                    <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">
    				                </a>";
    			}
    			$htm .= $boExcluir." Arquivo em anexo: <a onclick=\"janela(window.location.href+'&req=download&arqid={$dadosResp['arqid']}', 600, 480);\">".$dadosResp['arqdescricao']."</a>";
    		} else {
        		$htm .= "<input type='hidden' name='salvar_questionario' id='salvar_questionario'     value='1'>";
        		$htm .= "<input type='hidden' name='perid'               id='perid'                   value=''>";
        		$htm .= "<input type='hidden' name='itpid_pai_{$perid}'  id='itpid_pai_{$perid}'      value='{$itemPaiPergunta->itpid}'>";
        		$htm .= "<input type='hidden' name='perid_pai_{$perid}'  id='perid_pai_{$perid}'      value='{$itemPaiPergunta->perid}'>";
        		$htm .= "<input type='hidden' name='req'                                              value='salvarArquivoQuestionario'>";
        		$htm .= "<input type='hidden' name='qrpid'               id='qrpid'                   value='{$qrpid}'>";
        		$htm .= "<input type='hidden' name='peridExterno[]'      id='peridExterno_{$perid}'   value='{$perid}'>";
        		$htm .= "<input type='hidden' name='arqid[{$perid}]'     id='arqid'                   value='{$dadosResp['arqid']}'>";
    			$htm .= "<input type='file'   name='arquivo_{$perid}'    id='arquivo_itpid_{$itpid}' />";
    		}
		}

  		$this->htm = $htm;

	}

	//Da echo no resultado do campo externo para alimentar o relatorio de respostas do questionario.
	function retornoRelatorio( $qrpid, $perid ){

		$percent = 90;
		$habilita = false;

		include_once APPRAIZ .'includes/classes/modelo/questionario/QQuestionarioResposta.class.inc';
		$obQestionario = new QQuestionarioResposta();
		$queid = $obQestionario->pegaQuestionario( $qrpid );

		$obQuestAnexo = new Par3_Model_QuestoesEstrategicasAnexos( );
		$dadosResp    = $obQuestAnexo->carregarResposta( $qrpid, $perid );

		$pergunta = new QPergunta($perid);
		$itemPaiPergunta = new QItemPergunta($pergunta->itpid);

        $arrEscolasCE     = array(4602, 4707, 4434, 4345, 4256, 4811);
        $arrGremio        = array(4605, 4710, 4437, 4348, 4259, 4814);
        $arrPNE           = array(4410, 4321, 4232);
        $arrPlanoEducacao = array(4231, 4320, 4409);
        $arrEscolasEPT    = array(4770, 4866);
        $arrEscolasEstEPT = array(4771, 4867);
        $arrEscolasProeja = array(4773, 4868);

		$htm = '';
		if (in_array($perid, $arrEscolasCE)) {
			$obQuestEscolaCE = new Par3_Model_QuestoesEstrategicasEscolasCe();
			$qdtEscolas      = $obQuestEscolaCE->retornarQtdEscolas( $qrpid, $perid );
			$htm .= "<h5>Selecione as escolas com CE em funcionamento
			<i class=\"fa fa-pencil-square-o escolas_ce\"
			style=\"font-size: 15px; cursor: pointer; color:green\"
			qrpid=$qrpid
			perid=$perid  ></i></h5>
			Total de escola da redemunicipal de ensino: <span id=total_escolas_ce >$qdtEscolas</span>";
		} else if (in_array($perid, $arrGremio)) {
			$obQuestEscolaGremio = new Par3_Model_QuestoesEstrategicasEscolasGremio();
			$qdtEscolas      = $obQuestEscolaGremio->retornarQtdEscolas( $qrpid, $perid );
			$htm .= "<h5>Selecione as escolas com Grêmio em funcionamento
			<i class=\"fa fa-pencil-square-o escolas_gremio\"
			style=\"font-size: 15px; cursor: pointer; color:green\"
			qrpid=$qrpid
			perid=$perid  ></i></h5>
			Total de escola da redemunicipal de ensino: <span id=total_escolas_gremio >$qdtEscolas</span>";
		} else if (in_array($perid, $arrPNE)) {
			$htm .= " <div clas=\"ibox-content\" style=\"text-align:justify;text-justify:inter-word;\">
    		                  O Plano Nacional de Educação (PNE), instituído pela Lei n° 13.005/2014,
    		              determinou, para o primeiro ano de vigência, a elaboração ou adequação dos
    		              planos estaduais, distrital e municipais de educação, em consonância com o
    		              texto nacional. No dia 24 de junho de 2015 venceu o prazo para todas as
    		              prefeituras, estados e o Distrito Federal finalizarem seus planos
    		              de educação, ou adequarem os planos já aprovados em lei às diretrizes
    		              do PNE. Se o seu estado/município ainda não publicou a lei com o plano
    		              de educação, poderá entrar em contato com a SASE (Secretaria de Articulação
    		              com os Sistema de Ensino), pelo e-mail XXXXXXXX, ou telefone(s) XXXXXXXXXX
    		          </div>";
		} else if (in_array($perid, $arrPlanoEducacao)) {
			$htm .= " Arquivo em anexo: <a onclick=\"janela(window.location.href+'&req=download&arqid={$dadosResp['arqid']}', 600, 480);\">
			{$dadosResp['arqdescricao']}</a><br />";
			if (!$dadosResp['arqid']) {
				$htm .= " <div clas=\"ibox-content\" style=\"color: red;text-align:justify;text-justify:inter-word;\">";
				$htm .= "         Caso o estado/município possua o plano Estadual/Municipal de Educação
        		              com a sua lei devidamente sancionada e o sistema não mostre o arquivo anexado,
        		              deverá entrar em contato com a equipe responsável na SASE(Secretaria de Articulação
        		              com os Sistemas de Ensino), pelo e-mail XX, ou telefone (x) xx";
				$htm .= " </div>";
			}
		} else if (in_array($perid, $arrEscolasEPT)) { #Escolas Exclusivas EPT
    		$obQuestEscolaEPT = new Par3_Model_QuestoesEstrategicasEscolasEPT();
    		$qdtEscolas      = $obQuestEscolaEPT->retornarQtdEscolasEPT( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas da rede estadual exclusivas de Educação Profissional e Tecnológica (EPT)
    		         <i class=\"fa fa-pencil-square-o escolas_ept\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escolas da rede estadual exclusivas de Educação Profissional e Tecnológica (EPT):
                         <span id=total_escolas_ept >$qdtEscolas</span>";
		} else if (in_array($perid, $arrEscolasEstEPT)) { #Escolas Estaduais que oferecem EPT
    		$obQuestEscolaEstEPT = new Par3_Model_QuestoesEstrategicasEscolasEstEPT();
    		$qdtEscolas      = $obQuestEscolaEstEPT->retornarQtdEscolasEstEPT( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas da rede estadual que oferecem Educação Profissional e Tecnológica (EPT)
    		         <i class=\"fa fa-pencil-square-o escolas_estept\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escolas da rede estadual que oferecem Educação Profissional e Tecnológica (EPT):
                         <span id=total_escolas_estept >$qdtEscolas</span>";
		} else if (in_array($perid, $arrEscolasProeja)) { #Escolas PROEJA
    		$obQuestEscolaProeja = new Par3_Model_QuestoesEstrategicasEscolasProeja();
    		$qdtEscolas      = $obQuestEscolaProeja->retornarQtdEscolasProeja( $qrpid, $perid );
    		$htm .= "<h5>Selecione as escolas da rede estadual com PROEJA
    		         <i class=\"fa fa-pencil-square-o escolas_proeja\"
    		            style=\"font-size: 15px; cursor: pointer; color:green\"
    		            qrpid=$qrpid
    		            perid=$perid  ></i></h5>
    		         Total de escolas da rede estadual com PROEJA:
                         <span id=total_escolas_proeja >$qdtEscolas</span>";
		} else {
			if( $dadosResp['arqid'] ){
				if ( $habilita ) {
					$boExcluir = "<a href=\"#\" onclick=\"excluirArquivo( ".$dadosResp['arqid'].", ".$perid." );\" title=\"Excluir Anexo\">
    				                    <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">
    				                </a>";
				} else {
					$boExcluir = "<a href=\"#\" title=\"Excluir Anexo\">
    				                    <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">
    				                </a>";
				}
				$htm .= $boExcluir." Arquivo em anexo: <a onclick=\"janela(window.location.href+'&req=download&arqid={$dadosResp['arqid']}', 600, 480);\">".$dadosResp['arqdescricao']."</a>";
			} else {
				$htm .= "Não informado.";
			}
		}

		echo $htm;
	}

	function salvar() {
		if( is_array($_POST['peridExterno']) ){
			foreach( $_POST['peridExterno'] as $perid ){
				$obQuest = new Par3_Model_QuestoesEstrategicasAnexos();

				if( $_FILES["arquivo_".$perid]["name"] ){
					$obQuest->verifica( $_POST['qrpid'], $perid, $_POST['inuid'][$perid] );

					$campos	= array("qrpid" => $_POST['qrpid'],
									"perid" => $perid);

					$file = new FilesSimec("questoesestrategicasanexos", $campos);
					if(is_file($_FILES["arquivo_".$perid]["tmp_name"])){
						$arquivoSalvo = $file->setUpload($_FILES["arquivo_".$perid]["name"], "arquivo_".$perid);
					}
				}
			}
		}
	}

	function show(){
		echo $this->htm;
	}
}

class CampoExternoControle extends Par3_Helper_CampoExterno {

}