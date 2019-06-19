<?php
/**
 * Classe de mapeamento da entidade sac.chamadohistorico
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */



/**
 * Sac__ModelChamadohistorico
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */
class Sac_Model_ChamadoHistorico extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.chamadohistorico';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'hicid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hicid' => null,
        'hicdsc' => null,
        'hicdata' => null,
        'usucpf' => null,
        'chaid' => null,
        'chastatus' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'hicid' => array(  'Digits'  ),
            'hicdsc' => array(  new Zend_Validate_StringLength(array('max' => 4000))  ),
            'hicdata' => array(  ),
            'usucpf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'chaid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)

    public function listarHistorico($chaid)
    {
        if($chaid){
            return "select ch.hicid,
                            coalesce(ha.arqid, -1) as arqid,
                            coalesce(
                               case
                                  when ch.chastatus = 'AB' then 'Aguardando Retorno do Suporte'
                                  when ch.chastatus = 'AV' then 'Aguardando Validação do Usuário'
                                  when ch.chastatus = 'AU' then 'Aguardando Retorno do Usuário'
                                  when ch.chastatus = 'FP' then 'Finalizado Positivo'
                                  when ch.chastatus = 'FN' then 'Finalizado Negativo'
                                  when ch.chastatus = 'FA' then 'Finalizado Automaticamente'
                                  else ch.chastatus
                               end
                           , substring(hicdsc, 0, 100) || '...') as status,
                           to_char(hicdata, 'DD/MM/YYYY HH:mi:ss') as hicdata, u.usucpf, u.usunome
                    from sac.chamadohistorico ch
                      inner join seguranca.usuario u on u.usucpf = ch.usucpf
                      left join  sac.historicoarquivo ha on ha.hicid = ch.hicid
                    where chaid = $chaid
                    order by hicdata desc, ch.hicid desc";
        }
        return false;
    }

    public function adicionarAnexo($arquivo = null, $hicid = null)
    {
        $arquivo = $arquivo ? $arquivo : $_FILES['arqid'];
        $hicid   = $hicid   ? $hicid   : $this->hicid;

        $arquivoSalvo = false;
        
        if($hicid){
            if ($arquivo['size'] > 0) {
                $campos = array("hicid" => "'" . $hicid . "'");
                $file = new FilesSimec("historicoarquivo", $campos, "sac");
                $arquivoSalvo = $file->setUpload($arquivo['name'], '', true);
            }
        }
        return $arquivoSalvo;
    }


}//end Class
?>