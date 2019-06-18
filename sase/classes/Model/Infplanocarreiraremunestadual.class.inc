<?php
/**
 * Sase_Model_Infplanocarreiraremunestadual
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */

class Sase_Model_Infplanocarreiraremunestadual extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.infplanocarreiraremunestadual';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'iplid',
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'iplid' => null,
        'ravid' => null,
        'spmid' => null,
        'iplinfomreunidas' => null,
        'iplqtdmunicipios' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'iplid' => array('Digits'),
            'ravid' => array('Digits'),
            'spmid' => array('Digits'),
            'iplinfomreunidas' => array('Digits'),
            'iplqtdmunicipios' => array('Digits')
        );
    }//end getCamposValidacao($dados)

    public function montaSqlInfoPlanoCarreiraRemuneracao($ravid, $tipo = 'n'){
        if(!$ravid) return array();
        $where = "";
        if($tipo == 'n'){
            $where = "where spm.spmid not in (1, 2)";
        } else {
            $where = "where spm.spmid in (1, 2)";
        }
        $sql = <<<DML
            select
                spm.spmid,
                spm.spmdsc,
                ipl.iplid,
                ipl.iplinfomreunidas,
                case
                    when ipl.iplqtdmunicipios is not null then ipl.iplqtdmunicipios
                    else (
                        select
                            count(marid)
                        from sase.municipiosassistidosredeae mar
                        inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                        inner join workflow.documento doc on pcp.docid2 = doc.docid
                        where mar.ravid = {$ravid}
                        and doc.esdid = spm.esdid
                    )
                end as iplqtdmunicipios
            from sase.sitplanomunicipio spm
            left join sase.infplanocarreiraremunestadual ipl on spm.spmid = ipl.spmid and ipl.ravid = {$ravid}
            {$where}
            order by spm.spmid
DML;
        return $sql;
    }

    public function getTabelaInfoPlanoCarreira($ravid, $perCampos = false){
        $sql = $this->montaSqlInfoPlanoCarreiraRemuneracao($ravid);
        if(!$sql){ return false; }
        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Etapas', 'Informações reunidas e descrição dos comprovantes anexados a este relatório'))
            ->esconderColunas(array('iplid', 'spmid', 'iplqtdmunicipios'))
            ->setCampos(array('spmdsc', 'iplinfomreunidas'))
            ->setFormOff()
            ->setLarguraColuna(array('spmdsc' => '50%', 'iplinfomreunidas' => '50%'));
        if($perCampos){
            $lista->addCallbackDeCampo('iplinfomreunidas', 'getIplinfomreunidas');
        }
        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function getTabelaMunAssistidos($ravid){
        $sql = $this->montaSqlInfoPlanoCarreiraRemuneracao($ravid);
        if(!$sql){ return false; }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Etapas', 'N° de municípios'))
            ->esconderColunas(array('iplid', 'spmid', 'iplinfomreunidas'))
            ->setCampos(array('spmdsc', 'iplqtdmunicipios'))
            ->addCallbackDeCampo('iplqtdmunicipios', 'getIplqtdmunicipios')
            ->setFormOff()
            ->setLarguraColuna(array('spmdsc' => '50%', 'iplinfomreunidas' => '50%'));
        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function getTableMunSemInformacao($ravid){
        $sql = $this->montaSqlInfoPlanoCarreiraRemuneracao($ravid, 's');
        if(!$sql){ return false; }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Etapas', 'N° de municípios'))
            ->esconderColunas(array('iplid', 'spmid', 'iplinfomreunidas'))
            ->setCampos(array('spmdsc', 'iplqtdmunicipios'))
            ->addCallbackDeCampo('iplqtdmunicipios', 'getIplqtdmunicipios')
            ->addCallbackDeCampo('spmdsc', 'getIplSpmid')
            ->setFormOff()
            ->setLarguraColuna(array('spmdsc' => '50%', 'iplinfomreunidas' => '50%'));
        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
}