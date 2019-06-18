<?php
/**
 * Classe de controle do  cfn_nutricionista_arquivos
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-05-2018
 * @link     no link
 */



/**
 * Par3_Controller_Cfn_nutricionista_arquivos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-05-2018
 * @link     no link
 */
class Par3_Controller_CfnNutricionistaArquivos extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_CfnNutricionistaArquivos($_GET['cfnid']);
//        $this->modelHistorico = new Par3_Model_Cfn_nutricionista_arquivosHistorico();
    }

    public function getListagemAjax($arrPost)
    {
//        ver($arrPost);
        $orderColumn = $arrPost['order'][0]['column'];
        $orderDir    = $arrPost['order'][0]['dir'];
        $res   = $this->model->getSqlListagemAjax($arrPost['length'],$arrPost['start'],$orderColumn,$orderDir);
        $data  = is_array($res)?$res:array();
        $response = [
            "draw"            => intval($_POST['draw']),
            "recordsTotal"    => intval($data[0]['full_count']),
            "data"            => array_map(function($arr){ return array_values($arr);},$data),
            "recordsFiltered" => intval($data[0]['full_count']),
            "start"           => $arrPost['start'],
            "length"          => $arrPost['length']
        ];
        return json_encode(simec_utf8_encode_recursive($response));
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;

        //Valida campos
        $erro = $this->model->validarInput($arrPost);
        if($erro){return $erro;}

        try{
            $file = new FilesSimec("cfn_nutricionista_arquivos", array(), "par3");
            $file->setUpload('Arquivo CFN Nutricionistas',false,false);
            $arqid = $file->getIdArquivo();

            //Valida se o arquivo posui todas as colunas
            if(!$this->model->validarCargaCfn($arqid,$arrPost['delimitador'])){
                $file->excluiArquivoFisico($arqid);
                $erros['erros']['arqid'] =  "Erro ao processar arquivo.Verifique se o caractere delimitador de colunas corresponde ao mesmo do arquivo.";
                return $erros;
            }

            $arrCfn['arqid']         = $arqid;
            $arrCfn['cfnobservacao'] = ($arrPost['cfnobservacao']);
            $arrCfn['cfncpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
            $this->model->popularDadosObjeto($arrCfn);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $processadoComSucesso = $this->model->processarCargaCfn($sucesso,$arqid,$arrPost['delimitador']);

            if($processadoComSucesso){
                return $sucesso;
            }
            throw new \Exception("A carga não foi executada pois o arquivo não está formatado corretamente");
        } catch (Exception $e) {
            $this->model->rollback();
            $actual_link = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
            simec_redirecionar($actual_link, 'error', $e->getMessage());
        }
    }

    public function listarHistoricoArquivos()
    {
        $sql = $this->model->getSqlListagem();

        $cabecalho = ["Arquivo", "Observação", "Data da importação","Usuário","Total de Registros"];
        $esconderColunas = ["arqid"];

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('listacfn');
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('download', 'downloadArquivo');
        $listagem->setAcaoComoCondicional('download', [['campo' => 'arqid', 'valor' => '{NULL}', 'op' => 'diferente']]);
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

        $lista = new Simec_Listagem();
    }
    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['cfnid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&cfnid={$id}";
        $cfn_nutricionista_arquivos = new Par3_Model_Cfn_nutricionista_arquivos($id);
        try{
            $cfn_nutricionista_arquivos->Xstatus = 'I';
            $cfn_nutricionista_arquivos->Xdtinativacao = date('Y-m-d H:i:s');
            $cfn_nutricionista_arquivos->Xusucpfinativacao = $_SESSION['usucpf'];

            $cfn_nutricionista_arquivos->salvar();
            $cfn_nutricionista_arquivos->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function gerarXlsNutricionistasNaoAtualizados($cfnid = null)
    {
        $arrNutricionistas = $this->model->carregarNutricionistasNaoAtualizados($cfnid);
        if(!$arrNutricionistas){
            simec_redirecionar('par3.php?modulo=principal/acompanhamento/analisarNutricionista&acao=A','error','O relatório não trouxe nenhum resultado');
        }
        $arrNutricionistas = array_filter($arrNutricionistas);
        if(count($arrNutricionistas) == 0){
            simec_redirecionar('par3.php?modulo=principal/acompanhamento/analisarNutricionista&acao=A','error','O relatório não trouxe nenhum resultado');
        }
        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/relatorio_nutricionistas_nao_atualizados.php';
    }

}
?>