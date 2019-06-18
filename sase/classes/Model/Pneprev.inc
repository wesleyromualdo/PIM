<?php
/**
 * Sase_Model_Pneprev
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */

class Sase_Model_Pneprev extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.pneprevdial';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ppdid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'subid' => array('tabela' => 'sase.pnedial', 'pk' => 'pneiddial'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ppdid' => null,
        'pneiddial' => null,
        'pnevalormeta' => null,
        'pnesemvalor' => null,
        'pnepossuiplanoremvigente' => null,
        'pneplanorefcaput' => null,
        'pneanoprevisto' => null,
        'pnecpfinclusao' => null,
        'pnedatainclusao' => null,
        'subid' => null,
        'pnemuncod' => null,
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'ppdid' => array(  'Digits'  ),
            'pneiddial' => array(  'Digits'  ),
            'pnevalormeta' => array( 'allowEmpty' => true ),
            'pnesemvalor' => array( 'allowEmpty' => true ),
            'pnepossuiplanoremvigente' => array( 'allowEmpty' => true ),
            'pneplanorefcaput' => array( 'allowEmpty' => true ),
            'pneanoprevisto' => array( 'allowEmpty' => true, 'Digits'  ),
            'pnecpfinclusao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'pnedatainclusao' => array( 'allowEmpty' => true ),
            'subid' => array(  'Digits'  ),
            'pnemuncod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))  ),
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

        try{
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            return false;
        }

        if($sucesso){
            return true;
        }
        return false;
    }//end gravar()

    public function carrega($pneid, $pndanoprevisto = ''){
        $and = empty($pndanoprevisto) ? "and pneanoprevisto = {$pndanoprevisto}" : '';
        $sql = <<<DML
            select ppdid from sase.pneprevdial where pneiddial = {$pneid} {$and}'
DML;
        $ppdid = self::pegaUm($sql);

        return new Sase_Model_Pneprev($ppdid);
    }

    public function carregaPorPneid($pneid){
        $sql = <<<DML
            select ppdid from sase.pneprevdial where pneiddial = {$pneid} order by pnedatainclusao desc
DML;
        $ppdid = self::pegaUm($sql);

        if($ppdid) {
            $this->carregarPorId($ppdid);
        }
    }


}