<?php
/**
 * Sase_Model_Municipiosseminftecredeae
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */

class Sase_Model_Municipiosseminftecredeae extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.municipiosseminftecredeae';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'msfid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'msfid' => null,
        'marid' => null,
        'esdid' => null,
        'mstobservacoes' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'msfid' => array('Digits'),
            'marid' => array('Digits'),
            'esdid' => array('Digits'),
            'mstobservacoes' => array('Digits')
        );
    }//end getCamposValidacao($dados)

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

    public function carrega($msfid)
    {
        return new Sase_Model_Municipiosseminftecredeae($msfid);
    }

    public function montaSqlRelatorio($ravid){
        $esdid = ESDID_SASE_PCP_SEM_INFORMACAO;
        $sql = <<<DML
            select
                mun.mundescricao,
                case
                    when esd.esddsc is null then 'Sem Informação*'
                    else esd.esddsc
                end esddsc,
                esd.esdid,
                msf.msfid,
                msf.mstobservacoes,
                mar.marid
            from sase.municipiosassistidosredeae mar
            inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
            inner join territorios.municipio mun on pcp.muncod = mun.muncod
            left join sase.municipiosseminftecredeae msf on msf.marid = mar.marid
            left join workflow.documento doc on pcp.docid2 = doc.docid
            left join workflow.estadodocumento esd on doc.esdid = esd.esdid
            where mar.ravid = {$ravid}
            and (esd.esdid = {$esdid} or esd.esdid is null)
            order by mun.mundescricao
DML;
        return $sql;
    }

    public function getTabelaRelatorio($ravid, $perCampos){
        if(!empty($ravid) || is_numeric($ravid)){
            $sql = $this->montaSqlRelatorio($ravid);

            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setQuery($sql)
                ->setCabecalho(array('Município', 'Etapa', 'Motivo da situação'))
                ->esconderColunas(array('esdid', 'marid', 'msfid'))
                ->setFormOff()
                ->setCampos(array('mundescricao', 'esddsc', 'mstobservacoes'));

            if($perCampos){
                $lista->addCallbackDeCampo('mstobservacoes', 'getObsMunicipiosSemInformacao');
            }

            $lista->setFormOff();

            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }else{
            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setDados($dados)
                ->setCabecalho(array('Município', 'Etapa', 'Motivo da situação'))
                ->setCampos(array('mundescricao', 'esddsc', 'mstobservacoes'))
                ->setFormOff();
            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }
}