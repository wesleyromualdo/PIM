<?php

/**
 * Classe de controle do  registrorelatorioobras
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14-06-2018
 * @link     no link
 */

/**
 * Par_Controller_Registrorelatorioobras
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 14-06-2018
 * @link no link
 */
class Par_Controller_Registrorelatorioobras
{

    private $model;

    public function __construct()
    {
        $this->model = new Par_Model_Registrorelatorioobras($_GET['rroid']);
    }

    function administraRequisicoes()
    {
        /* ADMINISTRA AS REQUISIÇÕES */
        if ($_POST["requisicao"] == 'gerar_relatorio') {
            if ($_POST["tipo_obra"] == 'PAC') {
                $this->administraPAC();
            } elseif ($_POST["tipo_obra"] == 'PAR') {
                $this->administraPAR();
            }
        }
        if ($_GET["requisicao"] == 'downloadXLS') {
            $arqid = $_GET["arqid"];
            if ($arqid) {
                $diretorio = $this->model->getDiretorioPorArqid($arqid);
                $nome = $this->model->getNomePorArqid($arqid);
                if ($diretorio != '') {
                    $this->geraExcelPeloDiretorio($diretorio, $nome);
                }
            } else {
                $this->retornaAlertErro('Erro ao gerar arquivo');
            }
        }
    }

    function listarUltimosRegistros($tipo = false)
    {
        if ($tipo) {
            $this->model->listarUltimosRegistros($tipo);
        } else {
            return false;
        }
    }

    /**
     * Função gravar
     * - grava os dados
     */
    public function salvar()
    {
        global $url;
        // $this->model = new Par_Controller_Registrorelatorioobras();
        $this->model->popularDadosObjeto();
        $url .= '&rroid=' . $this->model->rroid;
        
        try {
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        
        if ($sucesso) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }

    function geraArquivo($htmlTabela, $tipo)
    {
        $arqid = $this->model->geraArquivo($htmlTabela, $tipo);
        
        if ($arqid) {
            return $arqid;
        } else {
            return '';
        }
    }

    /**
     * $funcção responsável por administrar a lógica da geração do relatório PAR
     */
    function administraPAR()
    {
        global $db;
        if (! $this->model->verificaSeExiste('PAR')) {
            
            $cabecalho = Array(
                "Preid",
                "Obrid",
                "Nome da Obra",
                "Tipo da Obra",
                "Município",
                "Estado",
                "Descrição da Subação",
                "Situação",
                "Situação Obras 2",
                "Usuário",
                "Data da Situação",
                "Ano da Obra",
                "Analista",
                "Valor da Contrapartida",
                "Valor MEC/FNDE",
                "Valor da Obra",
                "Processo"
            );
            $arrRetorno = $this->model->retornaArrayRegistrosPAR();
            
            if (count($arrRetorno) > 0) {
                $htmlTabela = $this->retornaHtmlTabela($arrRetorno, $cabecalho);
                if ($htmlTabela) {
                    $arqid = $this->geraArquivo($htmlTabela, 'PAR');
                    if ($arqid) {
                        $resultado = $this->model->salvarRegistro($arqid, 'PAR');
                        if ($resultado) {
                            $nomeArquivo = $this->model->getRegistro(null, 'PAR');
                            if ($nomeArquivo) {
                                
                                $diretorio = APPRAIZ . "arquivos/par/relatorioacompanhamentoobras/{$nomeArquivo}";
                                $this->geraExcelPeloDiretorio($diretorio, $nomeArquivo);
                            } else {
                                $this->retornaAlertErro('Erro ao gerar arquivo');
                            }
                        }
                    } else {
                        $this->retornaAlertErro('erro na hora');
                    }
                } else {
                    $this->retornaAlertErro('Erro ao gerar relatório');
                }
            } else {
                $this->retornaAlertErro('Erro ao gerar relatório');
            }
        } else {
            
            $nomeArquivo = $this->model->getRegistro(null, 'PAR');
            
            if ($nomeArquivo) {
                
                $diretorio = APPRAIZ . "arquivos/par/relatorioacompanhamentoobras/{$nomeArquivo}";
                $this->geraExcelPeloDiretorio($diretorio, $nomeArquivo);
            } else {
                $this->retornaAlertErro('Erro ao gerar arquivo');
            }
        }
    }

    /**
     * $funcção responsável por administrar a lógica da geração do relatório PAC
     */
    function administraPAC()
    {
        global $db;
        if (! $this->model->verificaSeExiste('PAC')) {
            
            $arrRetorno = $this->model->retornaArrayRegistrosPAC();
            $cabecalho = Array(
                "Preid",
                "Obrid",
                "Nome da obra",
                "Tipo da obra",
                "Município",
                "UF",
                "Situação",
                "Situação - Obras 2.0",
                "Usuário",
                "Data da Situação",
                "Analista",
                "Resolução/Carga",
                "Valor da contrapartida",
                "Valor FNDE",
                "Valor da Obra",
                "Processo"
            );
            
            if (count($arrRetorno) > 0) {
                
                $htmlTabela = $this->retornaHtmlTabela($arrRetorno, $cabecalho);
                if ($htmlTabela) {
                    $arqid = $this->geraArquivo($htmlTabela, 'PAC');
                    if ($arqid) {
                        $resultado = $this->model->salvarRegistro($arqid, 'PAC');
                        if ($resultado) {
                            $nomeArquivo = $this->model->getRegistro(null, 'PAC');
                            if ($nomeArquivo) {
                                
                                $diretorio = APPRAIZ . "arquivos/par/relatorioacompanhamentoobras/{$nomeArquivo}";
                                $this->geraExcelPeloDiretorio($diretorio, $nomeArquivo);
                            } else {
                                $this->retornaAlertErro('Erro ao gerar arquivo');
                            }
                        }
                    } else {
                        $this->retornaAlertErro('erro na hora');
                    }
                } else {
                    $this->retornaAlertErro('Erro ao gerar relatório');
                }
            } else {
                $this->retornaAlertErro('Erro ao gerar relatório');
            }
        } else {
            
            $nomeArquivo = $this->model->getRegistro(null, 'PAC');
            if ($nomeArquivo) {
                
                $diretorio = APPRAIZ . "arquivos/par/relatorioacompanhamentoobras/{$nomeArquivo}";
                $this->geraExcelPeloDiretorio($diretorio, $nomeArquivo);
            } else {
                $this->retornaAlertErro('Erro ao gerar arquivo');
            }
        }
    }

    function retornaAlertErro($mensagem)
    {
        echo "<script>
                alert('$mensagem');
                window.location.href = 'par.php?modulo=relatorio/relatorioAcompanhamentoObras&acao=A';
            </script>";
        die();
    }

    function geraExcelPeloDiretorio($diretorio, $nome = null)
    {
        if (is_file($diretorio)) {
            
            ob_clean();
            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/xls; name=SIMEC_Relat" . date("Ymdhis") . ".xls");
            if ($nome != null) {
                header("Content-Disposition: attachment; filename={$nome}.xls");
            } else {
                header("Content-Disposition: attachment; filename=SIMEC_RelatPAR" . date("Ymdhis") . ".xls");
            }
            
            header("Content-Description: MID Gera excel");
            
            $texto = file_get_contents($diretorio);
        } else {
            
            $this->retornaAlertErro('Erro ao gerar relatório. Arquivo não encontrado');
        }
        if ($texto != '') {
            echo $texto;
            die();
        } else {
            ob_clean();
            $this->retornaAlertErro('Erro ao gerar relatório');
        }
    }

    /**
     * Função excluir
     * - grava os dados
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['rroid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&rroid={$id}";
        $registrorelatorioobras = new Par_Model_Registrorelatorioobras($id);
        try {
            $registrorelatorioobras->Xstatus = 'I';
            $registrorelatorioobras->Xdtinativacao = date('Y-m-d H:i:s');
            $registrorelatorioobras->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $registrorelatorioobras->salvar();
            $registrorelatorioobras->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    function retornaHtmlTabela($arrRegistros, $cabecalho)
    {
        $arrRegistros = is_array($arrRegistros) ? $arrRegistros : Array();
        $cabecalho = is_array($cabecalho) ? $cabecalho : Array();
        
        if ((count($arrRegistros) > 0) && (count($arrRegistros) > 0)) {
            $tabela .= "<table border='1'>
		    		<thead>
		    			<tr>";
            foreach ($cabecalho as $campo) {
                $tabela .= "
	    					<td>{$campo}</td>";
            }
            $tabela .= "		</tr>
		    		</thead>";
            $tabela .= "
	    			<tbody>";
            
            foreach ($arrRegistros as $linha) {
                $tabela .= "
	    				<tr>";
                
                foreach ($linha as $campo) {
                    $tabela .= "<td>
	    						{$campo}
	    					</td>";
                }
                
                $tabela .= "</tr>";
            }
            $tabela .= "
	    			</tbody>
	    		</table>
				";
            
            return $tabela;
        } else {
            return false;
        }
    }
}
?>