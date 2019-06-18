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

class Sase_Model_Espacoparticipacaoestado extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.espacoparticipacaoestado';

    /**
     * @var FilesSimec Referência para a classe de gestão de arquivos.
     */
    protected $arquivo;

    /**
     * Chave primaria.
     * @var array+ 
     * @access protected
     */
    protected $arChavePrimaria = array(
        'epeid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'epeid' => null,
        'estuf' => null,
        'sepidforum' => null,
        'sepidcons' => null,
        'sepidsist' => null,
        'usucpf' => null,
        'docid1' => null,
        'docid2' => null,
        'docid3' => null,
        'epestatus' => null,
        'epeforum' => null,
        'epeconselho' => null,
        'epesistema' => null
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

    public function montaSqlLista($estuf = ""){
        $where = array();
        if(!empty($estuf)){$where[] = " and esp.estuf = '{$estuf}'";}
        $where = implode(" ", $where);
        $sql = <<<DML
            select
                epe.epeid,
                epe.estuf,
                est.estdescricao,
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
                epeforum,
                epeconselho,
                epesistema
            from sase.espacoparticipacaoestado  epe
            left join sase.sitespparticipacao sepforum on epe.sepidforum = sepforum.sepid
            left join sase.sitespparticipacao sepcons on epe.sepidcons = sepcons.sepid
            left join sase.sitespparticipacao sepsist on epe.sepidsist = sepsist.sepid
            inner join territorios.estado est on epe.estuf = est.estuf
            where 1=1 {$where}
            order by est.estuf, est.estdescricao
DML;
        return $sql;
    }

    public function getTabelaLista($estuf = ""){
        $sql = $this->montaSqlLista($estuf);
        if(empty($sql)){return false;}

        $lista = new Simec_Listagem();
        $lista
            ->setQuery($sql)
            ->setTamanhoPagina(50)
            ->setCabecalho(array('UF', 'Estado', 'Situaçao Forum', 'Situaçao Conselho', 'Situação Sistema', 'Forum', 'Conselho', 'Sistema'))
            ->setCampos(array('estuf', 'estdescricao', 'sepforum', 'sepcons', 'sepsist', 'epeforum', 'epeconselho', 'epesistema'))
            ->setId('tbEspacodeparticipacao')
            ->esconderColunas(array('epeid'))
            ->addCallbackDeCampo('epeforum', 'verificaArquivoLista')
            ->addCallbackDeCampo('epeconselho', 'verificaArquivoLista')
            ->addCallbackDeCampo('epesistema', 'verificaArquivoLista')
            ->addAcao('edit', array(
                'func' => 'abreCadastro',
                'extra-params' => array(
                    'epeid'
                )
            ));

        return $lista->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function salvaArquivos(){
        try{
            if($_FILES['epeforum']) {
                if (UPLOAD_ERR_OK == $_FILES['epeforum']['error']) {
                    $arquivo = new FilesSimec("espaço de participaçao estado - forum - epeid: {$_POST['epeid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['epeforum']['name'], 'epeforum', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $this->popularDadosObjeto(['epeid' => $_POST['epeid'], 'epeforum' => $idArquivo]);
                        if (!$this->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $this->commit();
                    }
                    unset($arquivo, $model);
                }
            }

            if($_FILES['epeconselho']) {
                if (UPLOAD_ERR_OK == $_FILES['epeconselho']['error']) {
                    $arquivo = new FilesSimec("espaço de participaçao estado - conselho - epeid: {$_POST['epeid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['epeconselho']['name'], 'epeconselho', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $this->popularDadosObjeto(['epeid' => $_POST['epeid'], 'epeconselho' => $idArquivo]);
                        if (!$this->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $this->commit();
                    }
                    unset($arquivo, $model);
                }
            }

            if($_FILES['epesistema']) {
                if (UPLOAD_ERR_OK == $_FILES['epesistema']['error']) {
                    $arquivo = new FilesSimec("espaço de participaçao estado - sistema - epeid: {$_POST['epeid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['epesistema']['name'], 'epesistema', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $this->popularDadosObjeto(['epeid' => $_POST['epeid'], 'epesistema' => $idArquivo]);
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