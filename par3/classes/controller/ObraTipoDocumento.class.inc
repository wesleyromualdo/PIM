<?php
/**
 * Classe de controle do  obra_tipo_documento
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   PHILIPE ALLAN ALMEIDA <philipe.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */


/**
 * Par3_Controller_ObraTipoDocumento
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipoDocumento extends Controle
{
    private $model;

    const URL = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentoAnexo";

    public function __construct()
    {
        $this->model = new Par3_Model_ObraTipoDocumento($_GET['otdid']);
        $this->modelHistorico = new Par3_Model_ObraTipoDocumentoHistorico();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        //Todas as extensões
        $arrPost['extid'] = is_array($arrPost['extid']) ? $arrPost['extid'] : array();
        in_array("t", $arrPost['extid'])?$arrPost['otdtodas_extensoes'] = 't':$arrPost['otdtodas_extensoes'] = 'f';
        $arrPost = $arrPost;
        $arrPost['otdanalise_textorico'] = isset($_POST['otdanalise_textorico'])?'S':'N';

        try {

            $this->model->popularDadosObjeto($arrPost);
            //Salva o registro
            $this->model->antesSalvar();
            $sucesso = $this->model->salvar();
            $this->model->commit();

            //inseri o upload
            $arqid = $this->inserirUpload($this->model->otdid);
            $this->model->arqid = $arqid;

            //Todas as extensões
            if (!in_array("t", $arrPost['extid'])) {
                $this->inserirExtensoes($arrPost['extid'], $this->model->otdid);
            }

            //Grava o histórico
            $this->modelHistorico->gravarHistorico($this->model, 'CREATE');

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::URL, 'error');
        }

        if ($sucesso) {
            simec_redirecionar(self::URL, 'success');
        }
        simec_redirecionar(self::URL, 'error');
    }

    /**
     * Função para Salvar Edição
     * - grava os dados
     *
     */
    public function editar()
    {
    	$_POST['extid'] = is_array($_POST['extid']) ? $_POST['extid'] : array();
        $_POST['otdid'] = $_POST['id'];
        $_POST['otdanalise_textorico'] = isset($_POST['otdanalise_textorico'])?'S':'N';

        //Todas as extensões
        in_array("t", $_POST['extid'])?$_POST['otdtodas_extensoes'] = 't':$_POST['otdtodas_extensoes'] = 'f';

        $this->model = new Par3_Model_ObraTipoDocumento($_POST['id']);
        $this->model->popularDadosObjeto($_POST);

        try {

            //Salva o registro
            $sucesso = $this->model->salvar();
            $this->model->commit();

            //inseri o upload caso tenha um novo
            $arqid = $this->inserirUpload($this->model->otdid);
            $this->model->arqid = $arqid;

            //Todas as extensões
            if (in_array("t", $_POST['extid'])) {
                //Deletar todas as extensões
                $sql = ("DELETE FROM par3.obra_tipo_documento_x_extensao WHERE otdid = {$this->model->otdid}");
                $this->model->carregar($sql);
            }else{
                $this->inserirExtensoes($_POST['extid'], $this->model->otdid, 'editar');
            }

            /**
             * Exclui o anexo atual, caso tenho selecionado outro.
             */
            if (isset($dados['idsExcluirAnexo'])) {
                foreach ($dados['idsExcluirAnexo'] as $idArquivo) {
                    $this->apagarArquivo($idArquivo);
                }
            }

            //Grava o histórico
            $this->modelHistorico->gravarHistorico($this->model, 'UPDATE');

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::URL, 'error');
        }

        if ($sucesso) {
            simec_redirecionar(self::URL, 'success');
        }

        simec_redirecionar(self::URL, 'error');
    }


    /**
     * Função Validar
     *
     * Valida se já existe Documentos Anexos com o nome informado
     *
     */
    public function validar($dados)
    {
        ob_clean();

        parse_str($dados['dados'], $dados);

        $retorno = [
            "status" => true,
            "mensagem" => "Validado com sucesso!"
        ];

        $otddsc = $dados['otddsc'];
        $otdid = $dados['otdid'];
        $condicao = '';

        if (strlen($otdid) > 0) {
            $condicao = " AND otdid <> {$otdid}";
        }

        $documentos = $this->model->pegaUm("SELECT otdid FROM par3.obra_tipo_documento WHERE otddsc = '{$otddsc}' " . $condicao);

        /**
         * Verificar se já existe as informações salvas
         */
        if ($documentos) {
            $retorno = [
                "status" => 'false',
                "mensagem" => "Não é possível incluir o Documento Anexo. Já existe Documento Anexo cadastrado com o nome informado!"
            ];
        }

        exit(simec_json_encode($retorno));
    }


    /**
     * Função excluir
     *
     */
    public function inativar($id)
    {

        $obraTipoDocumento = new Par3_Model_ObraTipoDocumento($id);

        try {
            $obraTipoDocumento->otdstatus = 'I';
            $obraTipoDocumento->salvar();
            $obraTipoDocumento->commit();
            $this->modelHistorico->gravarHistorico($obraTipoDocumento, 'DELETE');
            simec_redirecionar(self::URL, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::URL, 'error');
        }
    }


    /**
     * Listagem dos documentos anexo
     *
     * @param null $arrPost
     */
    public function listar($arrPost)
    {

    	$arrPost['acao'] = $arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir' ? 'pesquisar' : $arrPost['acao'];
        $sql = $this->model->retornaSQLListaSimples($arrPost);

        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código', 'Descrição', 'Situacao', 'Extensão', 'Tamanho Máximo', 'Quatidade', 'Análise de Engenharia');
        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : ($arrPost['requisicao'] == 'imprimir' ? Simec_Listagem::RELATORIO_CORRIDO : Simec_Listagem::RELATORIO_PAGINADO);
        $esconderColunas = array('qtd');
        //print_r($tipoRelatorio); exit();
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        
        if($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir'){
        	array_unshift($cabecalho,'QTD');
        	$esconderColunas = array('id');
        	$dados = $this->model->carregar($sql);
        	$dadosTratados = array();
        }


        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('modsituacao', 'formata_status');
        if($arrPost['requisicao'] != 'xls' && $arrPost['requisicao'] != 'imprimir'){
            $listagem->addAcao('remove', 'removerDocumentoAnexo', array('titulo' => 'Remover'));
            $listagem->addAcao('edit', 'editarDocumentoAnexo', array('titulo' => 'Alterar'));
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }


    /**
     * Controla as requisição Json
     *
     * @param $opcao
     * @return string
     */
    public function controlarRequisicaoJson($opcao)
    {

        switch ($opcao) {

            //Dados do documento anexo.
            case "documentoanexo":
                ob_clean();
                $otdid = $_GET['otdid'];
                $retorno = $this->listarDadosDocumentoAnexo($otdid);
                break;

            //Dados do documento anexo.
            case "extensoes":
                ob_clean();
                $retorno = $this->listarExtensoes();
                break;

            default:
                ob_clean();
                $retorno = ['retorno' => 'Opção não encontrada ou inválida.'];
        }

        //Verifica se retornou falso.
        if ($retorno === false) {
            $retorno = [];
        }

        return simec_json_encode($retorno);

    }

    /**
     * Função para inserir upload.
     *
     * @param $otdid
     * @return mixed
     */
    public function inserirUpload($otdid)
    {
        $file = new FilesSimec("obra_tipo_documento", array('otdid' => $otdid), "par3");
        $file->setUpload('Documentos Anexos', 'anexo', false);
        $dados['arqid'] = $file->getIdArquivo();

        /**
         * Vincula o id do documento na tabela obra tipo documento.
         */
        $model = new Par3_Model_ObraTipoDocumento($otdid);
        $model->popularDadosObjeto($dados);
        $model->salvar();
        $model->commit();

        return $dados['arqid'];
    }


    /**
     * Função para inserir as extenções.
     *
     * @param $extensoes
     * @param $otdid
     * @param bool $acao
     * @return bool
     */
    public function inserirExtensoes($extensoes, $otdid, $acao = false)
    {

        try {

            //Se for editar, apaga todos os registro.
            if (isset($acao) && $acao == 'editar') {
                //Deletar todas
                $sql = ("DELETE FROM par3.obra_tipo_documento_x_extensao WHERE otdid = {$otdid}");
                $this->model->carregar($sql);
            }

            //Inseri os registros
            if (count($extensoes) > 0) {

                foreach ($extensoes as $extid) {

                    $sql = (" INSERT INTO par3.obra_tipo_documento_x_extensao(
                                extid,
                                otdid
                            )
                            VALUES (
                                {$extid},
                                {$otdid}
                            )");

                    $this->model->carregar($sql);
                }
            }

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::URL, 'error');
        }

        return true;

    }


    /**
     * Lista de extenções de arquivos
     *
     * @return array|mixed|NULL
     */
    public function listarExtensoes()
    {
        $retorno = $this->model->carregar("
            SELECT  extid as codigo,
                    extdesc as descricao
            FROM extensao_arquivo
        ");

        if (!$retorno) {
            $retorno = array();
        }

        return $retorno;

    }

    /**
     * Lista dos dados do Documento anexo
     *
     * @param $otdid
     * @return mixed
     */
    public function listarDadosDocumentoAnexo($otdid)
    {
        $retorno = $this->model->carregar("
                  SELECT  * FROM par3.obra_tipo_documento WHERE otdid = {$otdid}
        ");

        //Extesões selecionadas
        $retorno[0]['extesoes'] = $this->model->carregar("
                  SELECT extid FROM par3.obra_tipo_documento_x_extensao WHERE otdid = {$otdid}
        ");

        //Todas as extensões
        $retorno[0]['listarExtensoes'] = $this->listarExtensoes();

        //Lista de anexos selecionados
        $retorno[0]['anexos'] = current($this->listaAnexosDocumento($retorno[0]['otdid']));

        if (!$retorno) {
            $retorno = array();
        }

        return current($retorno);

    }

    /**
     * Lista os Anexos do Documento
     *
     * @param $otdid
     * @return mixed
     */
    public function listaAnexosDocumento($otdid)
    {
        $sql = "SELECT 	arquivo.arqid,
                        arqnome,
                        arqdescricao,
                        arqextensao,
                        arqtipo,
                        arqtamanho,
                        arqdata,
                        arqhora,
                        arqstatus,
                        usu.usucpf,
                        usu.usunome,
                        sisid
                    FROM arquivo
                    INNER JOIN par3.obra_tipo_documento
                      ON (obra_tipo_documento.arqid = arquivo.arqid)
                    LEFT JOIN seguranca.usuario usu
                      ON usu.usucpf = arquivo.usucpf
                    WHERE arqstatus = 'A' AND obra_tipo_documento.otdid = {$otdid}";

        $retorno = $this->model->carregar($sql);

        if (!$retorno) {
            $retorno = array();
        }

        return $retorno;
    }

    /**
     * Verificar Descrição
     *
     * Sistema valida se já existe Documentos Anexos com o nome informado
     *
     * @param $descricao
     * @param null $otdid
     * @return array|mixed|NULL
     */
    public function verificarDescricao($descricao, $otdid = null)
    {
        $sql = "SELECT otdid FROM par3.obra_tipo_documento WHERE otddsc iLIKE '%{$descricao}%'";

        $retorno = $this->model->carregar($sql);

        if (!$retorno) {
            $retorno = array();
        }

        return $retorno;
    }

    /**
     * Responsável por excluir anexo.
     *
     * @param $arqId
     * @return bool
     */
    public function apagarArquivo($arqId)
    {
        $file = new FilesSimec("obra_tipo_documento", null, 'par3');
        $file->excluiArquivoFisico($arqId);
        $this->banco->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = {$arqId}");
        $this->banco->commit();

        return true;
    }

    public function retornarDocumentos($dados)
    {
        $sql = retornaSQLListaSimples($dados);

        return $this->model_>carregar($sql);
    }


}

?>