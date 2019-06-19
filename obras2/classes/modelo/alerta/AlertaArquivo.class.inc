<?php

class AlertaArquivo extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.alerta_arquivo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("aaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aaid' => null,
        'aid' => null,
        'arqid' => null
    );

    public function buscaArquivosPorAlerta($id_alerta)
    {
        if(!$id_alerta) return false;
        $sql = <<<DML
            SELECT
                arq.arqid, arq.arqnome, arq.arqdescricao, arq.arqextensao
            FROM {$this->stNomeTabela} aa
            INNER join public.arquivo arq ON aa.arqid = arq.arqid
            WHERE aa.aid = $id_alerta
                AND arq.arqstatus = 'A';
DML;
        return $this->carregar($sql);
    }

    public function salvarAlertaArquivo($dados,$id_alerta)
    {
        $arrArqid = $dados['arquivo_alerta'] ? $dados['arquivo_alerta'] : array();
        if ($_FILES['arquivo_alerta']['name'][0] != '') {
            $arquivos = $_FILES;
            foreach ($arquivos['arquivo_alerta']['name'] as $key => $value) {
                if(empty($value))
                    continue;

                $files =  array(
                    'name' => $arquivos['arquivo_alerta']['name'][$key],
                    'type' => $arquivos['arquivo_alerta']['type'][$key],
                    'tmp_name' => $arquivos['arquivo_alerta']['tmp_name'][$key],
                    'error' => $arquivos['arquivo_alerta']['error'][$key],
                    'size' => $arquivos['arquivo_alerta']['size'][$key]
                );
                $_FILES['arquivo'] = $files;
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload(NULL, 'arquivo', false);
                $arqid = $file->getIdArquivo();
                if ($arqid) {
                    $arrArqid[] = $arqid;
                    $this->popularDadosObjeto(array('aaid' => NULL,'arqid' => $arqid, 'aid' => $id_alerta));
                    $this->salvar();
                }
            }
            $this->commit();
        }
        $this->verificaArqid($id_alerta, $arrArqid);
    }

    function verificaArqid($id, $arqid)
    {
        $arqid = (is_array($arqid)) ? $arqid : array();
        $sql = "
            SELECT al.arqid
            FROM obras2.alerta_arquivo al
            JOIN arquivo a ON a.arqid = al.arqid
            WHERE al.aid = $id
        ";
        $arquivos  = $this->carregar($sql);

        if(!empty($arquivos)) {
            foreach ($arquivos as $arquivo) {
                if (array_search($arquivo['arqid'], $arqid) === false) {
                    $sql = "DELETE FROM obras2.alerta_arquivo WHERE arqid = {$arquivo['arqid']}";
                    $this->executar($sql);
                }
            }
            $this->commit();
        }
    }


}