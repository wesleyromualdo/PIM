<?php
/**
 * Created by PhpStorm.
 * User: danielfiuza
 * Date: 30/10/17
 * Time: 15:39
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_aba_perguntas.
 * @version $Id$
 * @since 2017.10.30
 */

/**
 * Par3_Model_AnaliseEngenhariaObraAbaPerguntas: utilizada para cadastro de perguntas para o questionário de análise de engenharia
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza.gov.br>
 *
 * @property int $aepid  - default: nextval('par3.analise_engenharia_obra_aba_perguntas'::regclass)
 * @property int $oabid
 * @property string $aepdsc
 * @property string $aepordem
 */
class Par3_Model_AnaliseEngenhariaObraAbaPerguntas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_aba_perguntas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aepid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'oabid' => array('tabela' => 'par3.obra_aba', 'pk' => 'oabid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aepid'    => null,
        'oabid'    => null,
        'aepdsc'   => null,
        'aepordem' => null,
    );

    public function __construct($oabid = null,$aepordem = null)
    {
        global $db;
        $id = null;
        if($oabid && $aepordem){
            $aepid = $db->recuperar("SELECT aepid FROM {$this->stNomeTabela} WHERE oabid = {$oabid} AND aepordem = {$aepordem}");
            $id = $aepid['aepid'];
        }
        parent::__construct($id);
    }
}