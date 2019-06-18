<?php
/**
 * Created by PhpStorm.
 * User: danielfiuza
 * Date: 31/10/17
 * Time: 17:42
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_aba_respostas.
 * @version $Id$
 * @since 2017.10.30
 */

/**
 * Par3_Model_AnaliseEngenhariaObraAbaRespostas: utilizada para cadastro de respostas para o questionário de análise de engenharia
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza.gov.br>
 *
 * @property int $aerid  - default: nextval('par3.analise_engenharia_obra_aba_respostas'::regclass)
 * @property int $aepid
 * @property int $aegid
 * @property char $aer_pendencia
 * @property string $aer_observacao
 * @property char $aer_corrigido
 */
class Par3_Model_AnaliseEngenhariaObraAbaRespostas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_aba_respostas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aerid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'aepid' => array('tabela' => 'par3.analise_engenharia_obra_aba_perguntas', 'pk' => 'aepid'),
        'aegid' => array('tabela' => 'par3.analise_engenharia_obra', 'pk' => 'aegid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aerid'          => null,
        'aegid'          => null,
        'aepid'          => null,
        'aer_pendencia'  => null,
        'aer_observacao' => null,
        'aer_corrigido'  => null,
    );

    public function __construct($aegid = null,$aepid = null)
    {
        global $db;
        $id = null;
        if($aegid && $aepid){
            $sql = "SELECT aerid FROM {$this->stNomeTabela} WHERE aegid = {$aegid} AND aepid = {$aepid}";
            $aerid = $db->recuperar($sql);
            $id = $aerid['aerid'];
        }
        parent::__construct($id);
    }

    public function recuperarObservacoesPorObraAba($obrid,$oabid)
    {
        $sql = "select * from par3.analise_engenharia_obra_aba_respostas resp
                inner join par3.analise_engenharia_obra_aba_perguntas per on per.aepid = resp.aepid
                inner join par3.analise_engenharia_obra aeg on aeg.aegid = resp.aegid
                where aeg.obrid = {$obrid} and per.oabid = {$oabid} and aeg.aeg_finalizado IS NULL";
        return $this->carregar($sql);
    }
}