<?php

class Par3_Controller_TipoDocumentoAnexo extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_TipoDocumentoAnexo();
    }

    public function desativar($doaid)
    {
        $tipoDocumentoAnexo = new Par3_Model_TipoDocumentoAnexo($doaid);

        try {
            $tipoDocumentoAnexo->doastatus = 'I';
            $tipoDocumentoAnexo->salvar();
            $tipoDocumentoAnexo->commit();
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error');
        }
    }

    public function editar(array $dados)
    {
        $this->model = new Par3_Model_TipoDocumentoAnexo($dados['doaid']);
        $this->model->popularDadosObjeto($dados);

        try {
            $this->model->removerTipoAtendimento($this->model->doaid);
            $this->model->removerExtensoes($this->model->doaid);
            $arquivosParaExclusao = [];

            if (array_key_exists('idsExcluirAnexo', $dados) && !empty($dados['idsExcluirAnexo'])) {
                $file = new FilesSimec();
                $file->setUpload('PAR3 - Tipo Documentos Anexos', null, false);
                $this->model->arqid = $file->getIdArquivo();
                $arquivosParaExclusao = $dados['idsExcluirAnexo'];
            }

            $this->model->salvar();

            if (array_key_exists('extid', $dados) && !empty($dados['extid'])) {
                $this->model->salvarExtensoes($this->model->doaid, $dados['extid']);
            }

            if (array_key_exists('tpaid', $dados) && !empty($dados['tpaid'])) {
                $this->model->salvarTipoAtendimento($this->model->doaid, $dados['tpaid']);
            }

            $this->excluirArquivos($arquivosParaExclusao);
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            $this->model->rollback();
            simec_redirecionar(self::URL, 'error');
        }
    }

    public function inserir(array $dados)
    {
        $file = new FilesSimec();
        $file->setUpload('PAR3 - Tipo Documentos Anexos', null, false);
        $this->model->popularDadosObjeto($dados);
        $this->model->arqid = $file->getIdArquivo();

        try {
            $doaid = $this->model->salvar();

            if (array_key_exists('extid', $dados) && !empty($dados['extid'])) {
                $this->model->salvarExtensoes($doaid, $dados['extid']);
            }

            if (array_key_exists('tpaid', $dados) && !empty($dados['tpaid'])) {
                $this->model->salvarTipoAtendimento($doaid, $dados['tpaid']);
            }

            $this->model->commit();
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'success');
        } catch (Simec_Db_Exception $e) {
            $this->model->rollback();
            $file->setRemoveUpload($this->model->arqid);
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error');
        }
    }

    public function listar($arrPost)
    {
        $sql = '
            SELECT
              doaid,
              doaid as codigo,
              doadescricao,
              CASE 
                WHEN doasituacao = \'A\'
                  THEN \'Ativo\'
                ELSE \'Inativo\'
              END AS doasituacao,
              COALESCE (NULLIF((
                array_to_string(
                  ARRAY(
                    SELECT extdesc
                    FROM par3.tipo_documento_anexo_extensao as tdae
                    INNER JOIN extensao_arquivo ext ON (tdae.extid = ext.extid)
                    WHERE tdae.doaid = tda.doaid
                  ), \', \'
                )
              ), \'\'), \'Todos\') AS extensoes,
              doatxtajuda,
              doaqtdmaxima,
              doatxtinstrucao
            FROM 
              par3.tipo_documento_anexo tda
            WHERE
              doastatus = \'A\'
        ';

        if (array_key_exists('codigo', $arrPost) && !empty($arrPost['codigo'])) {
            $sql .= ' AND doaid = ' . $arrPost['codigo'];
        }

        if (array_key_exists('descricao', $arrPost) && !empty($arrPost['descricao'])) {
            $sql .= " AND doadescricao = '" . $arrPost['descricao'] . "'";
        }

        $cabecalho = ['Código', 'Descrição', 'Situação', 'Extensões', 'Texto de ajuda', 'Quatidade máxima', 'Texto de instrução'];

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('remove', 'removerTipoDocumentoAnexo', array('titulo' => 'Remover'));
        $listagem->addAcao('edit', 'editarTipoDocumentoAnexo', array('titulo' => 'Alterar'));
        $listagem->turnOnOrdenacao();
        $listagem->render();
    }

    public function controlarRequisicaoJson($opcao)
    {
        switch ($opcao) {
            case "extensoes":
                ob_clean();
                $retorno = $this->listarExtensoes();
                break;

            case "tipoatendimentos":
                ob_clean();
                $retorno = $this->listarTipoAtendimentos();
                break;

            case "buscaTipoDocumentoAnexo":
                $doaid = $_GET['doaid'];
                ob_clean();
                $retorno = $this->buscaTipoDocumentoAnexo($doaid);
                break;

            case "buscardescricaoexistente":
                $doadescricao = $_POST['doadescricao'];
                $doaid = $_POST['doaid'];
                ob_clean();
                $retorno = $this->buscaTipoDocumentoAnexoPorDescricao($doaid, $doadescricao);
                break;

            default:
                ob_clean();
                $retorno = ['retorno' => 'Opção não encontrada ou inválida.'];
        }

        if ($retorno === false) {
            $retorno = [];
        }

        return simec_json_encode($retorno);
    }

    public function listarExtensoes()
    {
        $retorno = $this->model->carregar('
            SELECT  
              extid as codigo,
              extdesc as descricao
            FROM 
              extensao_arquivo
        ');

        if (!$retorno) {
            $retorno = [];
        }

        return $retorno;
    }

    public function listarTipoAtendimentos()
    {
        $retorno = $this->model->carregar('
            SELECT  
              tpaid as codigo,
              tpadesc as descricao
            FROM 
              par3.tipo_atendimento
        ');

        if (!$retorno) {
            $retorno = [];
        }

        return $retorno;
    }

    public function listaAnexosDocumento($doaid)
    {
        $sql = '
          SELECT 	
            arquivo.arqid,
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
            INNER JOIN par3.tipo_documento_anexo
              ON (tipo_documento_anexo.arqid = arquivo.arqid)
            LEFT JOIN seguranca.usuario usu
              ON usu.usucpf = arquivo.usucpf
          WHERE 
            arqstatus = \'A\' 
            AND tipo_documento_anexo.doaid = ' . $doaid;

        $retorno = $this->model->carregar($sql);

        if (!$retorno) {
            $retorno = [];
        }

        return $retorno;
    }

    protected function buscaTipoDocumentoAnexo($doaid)
    {
        $retorno = $this->model->carregar('SELECT * FROM par3.tipo_documento_anexo WHERE doaid = ' . $doaid);
        $retorno[0]['extensoes'] = $this->model->carregar('SELECT extid FROM par3.tipo_documento_anexo_extensao WHERE doaid = ' . $doaid);
        $retorno[0]['listarExtensoes'] = $this->listarExtensoes();
        $retorno[0]['tipoAtendimentos'] = $this->model->carregar('SELECT tpaid FROM par3.tipo_atendimento_documentos_anexos WHERE doaid = ' . $doaid);
        $retorno[0]['listarTipoAtendimentos'] = $this->listarTipoAtendimentos();
        $retorno[0]['anexos'] = current($this->listaAnexosDocumento($doaid));

        if (!$retorno) {
            $retorno = [];
        }

        return current($retorno);
    }

    protected function buscaTipoDocumentoAnexoPorDescricao($doaid, $doadescricao)
    {
        $doadescricao = strtolower(removeAcentos(($doadescricao)));
        $sql = sprintf('SELECT doaid, doadescricao FROM par3.tipo_documento_anexo WHERE lower(removeacento(doadescricao)) = \'%s\' AND doaid <> \'%d\'', $doadescricao, $doaid);

        $retorno = $this->model->carregar($sql);

        if (!$retorno) {
            $retorno = [];
        }

        return current($retorno);
    }

    protected function excluirArquivos(array $idsExcluirAnexo)
    {
        if(!empty($idsExcluirAnexo)) {
            $file = new FilesSimec();
            $file->setPulaTableEschema(true);

            foreach ($idsExcluirAnexo as $arqidExclusao) {
                $file->setRemoveUpload($arqidExclusao);
            }
        }
    }

    /**
     * Apenas para documentos que possuem análise
     * @return string
     */
    public function sqlComboDocumentosAnalise(){
        $sql = "SELECT
                        doaid AS codigo,
                        doadescricao AS descricao
                FROM par3.tipo_documento_anexo
                WHERE
                  doastatus = 'A'
                AND doaanalise IS TRUE ORDER BY doadescricao ASC";

        return $sql;
    }
}