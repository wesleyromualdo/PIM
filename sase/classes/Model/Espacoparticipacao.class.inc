<?php
/**
 * Sase_Model_Espacoparticipacao
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Espacoparticipacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.espacoparticipacao';

    /**
     * @var FilesSimec Referência para a classe de gestão de arquivos.
     */
    protected $arquivo;

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'espid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'espid' => null,
        'estuf' => null,
        'muncod' => null,
        'sepidforum' => null,
        'sepidcons' => null,
        'sepidsist' => null,
        'usucpf' => null,
        'docid1' => null,
        'docid2' => null,
        'docid3' => null,
        'espstatus' => null,
        'espforum' => null,
        'espconselho' => null,
        'espsistema' => null
    );

    /**
     * Retorna uma referencia para FilesSimec.
     *
     * @return FilesSimec
     */
    protected function getArquivo()
    {
        if (is_null($this->arquivo)) {
            $this->arquivo = new FilesSimec();
        }

        return $this->arquivo;
    }

    /**
     * Retorna informações de um arquivo.
     *
     * @param string $campoArquivo Nome do atributo que guarda o ID do arquivo.
     * @return array
     */
    public function getInfoArquivo($campoArquivo)
    {
        if (empty($this->$campoArquivo)) {
            return [];
        }
        return $this
            ->getArquivo()
            ->getDadosArquivo($this->$campoArquivo);
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        $this->popularDadosObjeto();

        try {
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            return false;
        }

        if ($sucesso) {
            return true;
        }
        return false;
    }//end gravar()

    public function montaSqlLista($estuf = "", $muncod = ""){
        $where = array();
        if(!empty($estuf)){$where[] = " and esp.estuf = '{$estuf}'";}
        if(!empty($muncod)){$where[] = " and esp.muncod = '{$muncod}'";}
        $where = implode(" ", $where);
        $sql = <<<DML
            select
                esp.espid,
                esp.estuf,
                mun.mundescricao,
                case
        			when sepforum.sepdsc is null then 'Sem Informação'
		        	else sepforum.sepdsc
		        end as sepforum,
                case
			        when sepcons.sepdsc is null then 'Sem Informação'
			        else sepcons.sepdsc
		        end as sepcons,
                case
			        when sepsist.sepdsc is null then 'Sem Informação'
			        else sepsist.sepdsc
		        end as sepsist,
                espforum,
                espconselho,
                espsistema
            from sase.espacoparticipacao esp
            left join sase.sitespparticipacao sepforum on esp.sepidforum = sepforum.sepid
            left join sase.sitespparticipacao sepcons on esp.sepidcons = sepcons.sepid
            left join sase.sitespparticipacao sepsist on esp.sepidsist = sepsist.sepid
            inner join territorios.municipio mun on esp.muncod = mun.muncod
            where 1=1 {$where}
            order by mun.estuf, mun.mundescricao
DML;
        return $sql;
    }

    public function getTabelaLista($estuf = "", $muncod = ""){
        $sql = $this->montaSqlLista($estuf, $muncod);
        if(empty($sql)){return false;}

        $lista = new Simec_Listagem();
        $lista
            ->setQuery($sql)
            ->setTamanhoPagina(50)
            ->setCabecalho(array('UF', 'Município', 'Situaçao Forum', 'Situaçao Conselho', 'Situação Sistema', 'Forum', 'Conselho', 'Sistema'))
            ->setCampos(array('estuf', 'mundescricao', 'sepforum', 'sepcons', 'sepsist', 'espforum', 'espconselho', 'espsistema'))
            ->setId('tbEspacodeparticipacao')
            ->esconderColunas(array('espid'))
            ->addCallbackDeCampo('espforum', 'verificaArquivoLista')
            ->addCallbackDeCampo('espconselho', 'verificaArquivoLista')
            ->addCallbackDeCampo('espsistema', 'verificaArquivoLista')
            ->addAcao('edit', array(
                'func' => 'abreCadastro',
                'extra-params' => array(
                    'espid'
                )
            ));

        return $lista->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function salvaArquivos(){
        try{
            if($_FILES['espforum']) {
                if (UPLOAD_ERR_OK == $_FILES['espforum']['error']) {
                    $arquivo = new FilesSimec("espaço de participaçao - forum - espid: {$_POST['espid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['espforum']['name'], 'espforum', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $this->popularDadosObjeto(['espid' => $_POST['espid'], 'espforum' => $idArquivo]);
                        if (!$this->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $this->commit();
                    }
                    unset($arquivo, $model);
                }
            }

            if($_FILES['espconselho']) {
                if (UPLOAD_ERR_OK == $_FILES['espconselho']['error']) {
                    $arquivo = new FilesSimec("espaço de participaçao - conselho - espid: {$_POST['espid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['espconselho']['name'], 'espconselho', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $this->popularDadosObjeto(['espid' => $_POST['espid'], 'espconselho' => $idArquivo]);
                        if (!$this->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $this->commit();
                    }
                    unset($arquivo, $model);
                }
            }

            if($_FILES['espsistema']) {
                if (UPLOAD_ERR_OK == $_FILES['espsistema']['error']) {
                    $arquivo = new FilesSimec("espaço de participaçao - sistema - espid: {$_POST['espid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['espsistema']['name'], 'espsistema', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $this->popularDadosObjeto(['espid' => $_POST['espid'], 'espsistema' => $idArquivo]);
                        if (!$this->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $this->commit();
                    }
                    unset($arquivo, $model);
                }
            }
        } catch(FileException $e) {
            echo 'Erro: ' . $e->getMessage();
        } catch(Exception $e){
            echo 'Erro: ' . $e->getMessage();
        }
    }

    public function getEstadoDocumento($docid){
        $sql = "select esdid From workflow.documento where docid = {$docid}";
        return $this->pegaUm($sql);
    }

    public function getSituacaoParticipacao($esdid){
        $sql = "select sepid From sase.sitespparticipacao where esdid = {$esdid}";
        return $this->pegaUm($sql);
    }

    public function validaDocid($docid, $tpdid){
        $estados = array(
            '260' => array('1737', '1738', '1739'),
            '261' => array('1732', '1733', '1734'),
            '262' => array('1742', '1743', '1744')
        );

        $esdid = $this->getEstadoDocumento($docid);

        return in_array($esdid, $estados[$tpdid]);
    }

}