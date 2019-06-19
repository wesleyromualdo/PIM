<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_programas_termo_adesao.
 *
 * @version $Id$
 * @since 2017.04.26
 */

/**
 * Par3_Model_Iniciativa_programas_termo_adesao: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_programas_termo_adesao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_programas_termo_adesao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $terid  - default: nextval('par3.iniciativa_programas_termo_adesao_terid_seq'::regclass)
 * @property int $inprgid 
 * @property int $tpdid 
 * @property string $terpacto 
 * @property string $tertermo 
 * @property string $teraceitepacto 
 * @property string $ternaoaceitepacto 
 * @property string $teraceitetermo 
 * @property string $ternaoaceitetermo 
 * @property string $teremailpreanalise 
 * @property int $teranoreferencia 
 * @property string $tercabecalhoadesao
 * @property string $teresfera
 * @property string $terinstrucoes 
 * @property string $termsgbemvindo 
 * @property string $terredirecionamentoadesao
 * @property string $inprgperiodoinicio
 * @property string $inprgperiodofim

 */
class Par3_Model_IniciativaProgramasTermoAdesao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_programas_termo_adesao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'terid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inprgid' => array('tabela' => 'par3.iniciativa_programas', 'pk' => 'inprgid'),
        'tpdid' => array('tabela' => 'workflow.tipodocumento', 'pk' => 'tpdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'terid'                     => null,
        'inprgid'                   => null,
        'tpdid'                     => null,
        'teresfera'                 => null,
        'terpacto'                  => null,
        'tertermo'                  => null,
        'teraceitepacto'            => null,
        'ternaoaceitepacto'         => null,
        'teraceitetermo'            => null,
        'ternaoaceitetermo'         => null,
        'teremailpreanalise'        => null,
        'teranoreferencia'          => null,
        'tercabecalhoadesao'        => null,
        'terinstrucoes'             => null,
        'termsgbemvindo'            => null,
        'terredirecionamentoadesao' => null,
        'terdtinclusao'             => null,
        'tercpfinclusao'            => null,
        'terdtinativacao'           => null,
        'tercpfinativacao'          => null,
        'terstatus'                 => null,
        'tersituacao'               => null,
    );

    public function montarSQLSimples($arrPost)
    {
        $sql = "SELECT 
                terid as id,
                terid as codigo,
                teranoreferencia,
                teresfera,
                terstatus
                FROM {$this->stNomeTabela} as ter
                INNER JOIN par3.iniciativa_programas as inpr ON ter.inprgid = inpr.inprgid
                WHERE terstatus = 'A' 
                AND  inpr.inprgstatus = 'A' 
                AND ter.inprgid = {$arrPost['inprgid']}
                ";
        return $sql;
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['teranoreferencia']      = array();
        $erros['erros']['terpacto']              = array();
        $erros['erros']['tertermo']              = array();
        $erros['erros']['teranoreferencia']      = array();
        $erros['erros']['teresfera']             = array();
        $erros['erros']['tercabecalhoadesao']    = array();

        //Validações
        if(9999 < (int)$campos['teranoreferencia'] ||(int)$campos['teranoreferencia'] < 1){array_push($erros['erros']['teranoreferencia'], 'Informe o ano com 4 inteiros e válida');}
        if(empty($campos['teranoreferencia']) || $campos['teranoreferencia'] == ''){array_push($erros['erros']['teranoreferencia'], 'Campo Obrigatório');}

        if(empty($campos['terpacto']) || $campos['terpacto'] == '<p><br></p>'){array_push($erros['erros']['terpacto'],    'Campo Obrigatório');}
        if(empty($campos['tertermo']) || $campos['tertermo'] == '<p><br></p>'){array_push($erros['erros']['tertermo'],    'Campo Obrigatório');}
        if(empty($campos['teresfera']) || $campos['teresfera'] == ''){array_push($erros['erros']['teresfera'], 'Campo Obrigatório');}
        if(empty($campos['tercabecalhoadesao']) || $campos['tercabecalhoadesao'] == ''){array_push($erros['erros']['tercabecalhoadesao'], 'Campo Obrigatório');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }
}
