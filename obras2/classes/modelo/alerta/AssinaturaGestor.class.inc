<?php

class AssinaturaGestor extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.assinatura_gestor";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("agid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'agid' => null,
        'nome' => null,
        'descricao' => null,
        'arqid' => null
    );

    public function buscaAssinatura($id)
    {
        if(!$id) return false;
        $sql = <<<DML
            SELECT
                ag.agid,
                ag.nome,
                ag.descricao,
                arq.arqid, arq.arqnome, arq.arqdescricao, arq.arqextensao
            FROM {$this->stNomeTabela} ag
            LEFT JOIN public.arquivo arq ON ag.arqid = arq.arqid AND arq.arqstatus = 'A'
            WHERE ag.agid = $id;
DML;
        return $this->pegaLinha($sql);
    }

    public function salvarAssinatura($dados)
    {
        if($_FILES['arqid']) {
            $file = new FilesSimec('arquivo', null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload(NULL, 'arqid', false);
            $arqid = $file->getIdArquivo();
        }
        if(!$arqid) {
            $arqid = $dados['arqid'];
        }
        $this->popularDadosObjeto(array('agid' => $dados['agid'], 'nome' => $dados['nome'], 'descricao' => $dados['descricao'], 'arqid' => $arqid));
        $this->salvar();
        $this->commit();
    }

    public function excluir($arqid)
    {
        if(!$arqid){
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        $sql = <<<DML
            UPDATE {$this->stNomeTabela} SET arqid = NULL WHERE arqid = $arqid;
DML;
        $this->executar($sql);
        $this->commit();
        $file = new FilesSimec('assinatura_gestor',$this->arAtributos,'obras2');
        $file->setPulaTableEschema(true);
        $file->setRemoveUpload($arqid);
    }
}